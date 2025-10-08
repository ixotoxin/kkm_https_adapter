// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "server_core.h"
#include "server_variables.h"
#include "server_strings.h"
#include "server_counter.h"
#include "server_hitman.h"
#include "server_default_handler.h"
#include "server_kkmop_handler.h"
#include "server_static_handler.h"
#include "server_config_handler.h"
#include "server_ping_handler.h"
#include "http_parser.h"
#include "http_request.h"
#include <cassert>
#include <utility>
#include <memory>
#include <atomic>
#include <latch>

namespace Server {
    static_assert(c_controlTimeout >= c_sleepQuantum);

    using namespace std::chrono_literals;

    namespace Wcs {
        using namespace Http::Wcs;
    }

    namespace Mbs {
        using namespace Http::Mbs;
    }

    using Basic::Failure;

    enum class State { Initial, Starting, Running, Shutdown, Stopping };

    static std::atomic<State> s_state { State::Initial };
    static Hitman s_hitman {};
    static std::latch s_shutdownSync { 2 };

    static Default::Handler s_defaultHandler {};
    static KkmOp::Handler s_kkmHandler {};
    static Static::Handler s_staticHandler {};
    static Config::Handler s_configHandler {};
    static Ping::Handler s_pingHandler {};

    [[nodiscard]]
    inline ProtoHandler & lookupHandler(const Http::Request & request) {
        // ISSUE: При большем количестве обработчиков стоит оптимизировать.
        if (request.m_hint.size() >= 2) {
            auto area = request.m_hint[1];
            if (area == "kkm") {
                return s_kkmHandler;
            } else if (area == "static") {
                return s_staticHandler;
            } else if (area == "config") {
                return s_configHandler;
            } else if (area == "ping") {
                return s_pingHandler;
            }
        }
        return s_defaultHandler;
    }

    template<typename CompletionToken>
    auto performAsync(const ProtoHandler & handler, Http::Request & request, CompletionToken && token) {
        return
            asio::async_compose<CompletionToken, void()>(
                [& handler, & request] (auto && self) {
                    std::thread(
                        [task = std::move(self), & handler, & request] () mutable { // NOLINT(*-move-forwarding-reference)
                            handler(request);
                            task.complete();
                        }
                    ).detach();
                },
                token
            );
    }

    asio::awaitable<void> accept(Asio::TcpSocket && socket, Asio::SslContext & sslContext) {
        Counter counter {};

        try {
            Asio::Stream stream { std::forward<Asio::TcpSocket>(socket), sslContext };
            Http::Request request { stream.lowest_layer().remote_endpoint().address() };

            try {
                { /** Приветствуем **/
                    const auto & [handshakeError]
                        = co_await stream.async_handshake(asio::ssl::stream_base::server);
                    if (handshakeError) {
                        request.m_response.m_status = Http::Status::BadRequest;
                        throw Failure(handshakeError); // NOLINT(*-exception-baseclass)
                    }
                }

                { /** Читаем запрос **/
                    Asio::StreamBuffer buffer {};
                    const auto & [headerReadingError, headerSize]
                        = co_await asio::async_read_until(stream, buffer, "\r\n\r\n");
                    if (headerReadingError) {
                        request.m_response.m_status = Http::Status::BadRequest;
                        throw Failure(headerReadingError); // NOLINT(*-exception-baseclass)
                    } else {
                        Http::Parser parser(request);
                        parser(buffer);
                        if (request.m_method == Http::Method::Post) {
                            auto expecting = parser.expecting();
                            while (expecting) {
                                const auto & [bodyReadingError, bodyReadingSize]
                                    = co_await asio::async_read(stream, buffer, asio::transfer_at_least(expecting));
                                if (bodyReadingError) {
                                    request.fail(Http::Status::BadRequest, bodyReadingError.message());
                                    break;
                                }
                                parser(buffer);
                                expecting = parser.expecting();
                            }
                        }
                        parser.complete();
                    }
                }

                { /** Обрабатываем запрос **/
                    if (request.m_response.m_status == Http::Status::Ok) {
                        LOG_INFO_TS(
                            [& request] {
                                std::string message {
                                    std::format(
                                        Mbs::c_requestedMethod,
                                        request.m_remote.to_string(),
                                        request.m_verb,
                                        request.m_path
                                    )
                                };
                                return std::format(Mbs::c_prefixedText, request.m_id, message);
                            }
                        );
                    }

                    if (
                        request.m_response.m_status < Http::Status::BadRequest
                        && (!s_loopbackWithoutSecret || !Asio::isLoopback(request.m_remote))
                    ) {
                        auto it = request.m_header.find("x-secret");
                        if (it == request.m_header.end() || it->second.empty() || it->second != s_secret) {
                            request.m_response.m_status = Http::Status::Forbidden;
                            request.m_response.m_data.emplace<1>(Mbs::c_forbidden);
                            LOG_ERROR_TS(Wcs::c_forbidden, request.m_id);
                        }
                    }

                    if (request.m_response.m_status == Http::Status::Ok) {
                        ProtoHandler & handler = lookupHandler(request);
                        if (handler.asyncReady()) {
                            co_await performAsync(handler, request, asio::use_awaitable);
                        } else {
                            (handler)(request);
                        }
                    }
                }

                { /** Пишем ответ **/
                    Asio::StreamBuffer buffer {};
                    request.m_response.render(buffer);
                    const auto & [writingError, writingSize] = co_await asio::async_write(stream, buffer);
                    if (writingError) {
                        request.m_response.m_status = Http::Status::InternalServerError;
                        LOG_ERROR_TS(writingError);
                    }
                }

                { /** Закрываем стрим и сокет **/
                    if (stream.lowest_layer().is_open()) {
                        Asio::Error error {};
                        stream.shutdown(error); // NOLINT(*-unused-return-value)
                        if (
                            error
                            && error != asio::ssl::error::stream_truncated  /* <= обязательно игнорируем */
                            /*&& error != asio::error::eof*/
                            && error != asio::error::connection_aborted     /* <= обязательно игнорируем */
                            && error != asio::error::connection_refused
                            && error != asio::error::connection_reset       /* <= обязательно игнорируем */
                            && error != asio::error::not_connected
                        ) {
                            LOG_WARNING_TS(Mbs::c_streamShutdownStatus, request.m_id, error.message());
                        }
                    }
                    if (stream.lowest_layer().is_open()) {
                        Asio::Error error {};
                        stream.lowest_layer().close(error); // NOLINT(*-unused-return-value)
                        if (error) {
                            LOG_WARNING_TS(Mbs::c_socketCloseStatus, request.m_id, error.message());
                        }
                    }
                }

            } catch (const Failure & e) {
                request.m_response.m_status = Http::Status::InternalServerError;
                LOG_ERROR_TS(e);
            } catch (const std::exception & e) {
                request.m_response.m_status = Http::Status::InternalServerError;
                LOG_ERROR_TS(e);
            } catch (...) {
                request.m_response.m_status = Http::Status::InternalServerError;
                LOG_ERROR_TS(Basic::Wcs::c_somethingWrong);
            }

            if (request.m_response.m_status < Http::Status::BadRequest) {
                LOG_INFO_TS(Wcs::c_prefixedText, request.m_id, Wcs::c_processingSuccess);
            } else {
                LOG_ERROR_TS(Wcs::c_prefixedText, request.m_id, Wcs::c_processingFailed);
            }

        } catch (const Failure & e) {
            LOG_ERROR_TS(e);
        } catch (const std::exception & e) {
            LOG_ERROR_TS(e);
        } catch (...) {
            LOG_ERROR_TS(Basic::Wcs::c_somethingWrong);
        }

        co_return;
    }

    asio::awaitable<void> listen() noexcept {
        assert(s_state.load() == State::Starting);

        try {
            auto port = static_cast<asio::ip::port_type>(s_port);
            Asio::Endpoint endpoint { s_ipv4Only ? asio::ip::tcp::v4() : asio::ip::tcp::v6(), port };
            Asio::SslContext sslContext { Asio::SslContext::tls_server };

            constexpr auto alwaysClear
                = Asio::SslContext::no_tlsv1_2 | Asio::SslContext::no_tlsv1_3;

            constexpr auto alwaysSet
                = Asio::SslContext::default_workarounds | Asio::SslContext::no_sslv2 | Asio::SslContext::no_sslv3;

            constexpr auto noLegacyTls
                = Asio::SslContext::no_tlsv1 | Asio::SslContext::no_tlsv1_1;

            if (s_enableLegacyTls) {
                ::SSL_CTX_set_security_level(sslContext.native_handle(), 0);
                ::SSL_CTX_set_min_proto_version(sslContext.native_handle(), TLS1_VERSION);
                sslContext.clear_options(alwaysClear | noLegacyTls);
                sslContext.set_options(alwaysSet);
            } else {
                if (s_securityLevel >= 0) {
                    ::SSL_CTX_set_security_level(sslContext.native_handle(), s_securityLevel);
                }
                sslContext.clear_options(alwaysClear);
                sslContext.set_options(alwaysSet | noLegacyTls);
            }

            sslContext.set_password_callback([] (auto, auto) { return s_privateKeyPassword; });
            // TODO: Облагородить способ установки ключа и сертификатов.
            sslContext.use_certificate_chain_file(Text::convert(s_certificateChainFile.native()));
            sslContext.use_private_key_file(Text::convert(s_privateKeyFile.native()), Asio::SslContext::pem);
            sslContext.set_verify_mode(asio::ssl::verify_none);

            auto executor = co_await asio::this_coro::executor;

            { /** Принимаем подключений **/
                Asio::Acceptor acceptor { executor, endpoint };

                LOG_INFO_TS(Wcs::c_started);
                s_state.store(State::Running);

                do {
                    auto [acceptingError, socket] = co_await acceptor.async_accept();
                    if (acceptingError) {
                        LOG_ERROR_TS(Mbs::c_connectionAcceptStatus, acceptingError.message());
                        LOG_ERROR_TS(Wcs::c_servicingFailed);
                    } else if (!socket.is_open()) {
                        LOG_ERROR_TS(Wcs::c_socketOpeningError);
                        LOG_ERROR_TS(Wcs::c_servicingFailed);
                    } else if (Counter::value() >= s_concurrencyLimit) {
                        socket.cancel();
                        socket.shutdown(Asio::TcpSocket::shutdown_both);
                        socket.close();
                        LOG_ERROR_TS(Wcs::c_maximumIsExceeded);
                    } else {
                        asio::co_spawn(executor, accept(std::move(socket), sslContext), asio::detached);
                    }
                } while (s_state.load() == State::Running);

                Asio::Error error {};
                acceptor.cancel(error); // NOLINT(*-unused-return-value)
                if (error) {
                    LOG_WARNING_TS(Mbs::c_acceptorCancelStatus, error.message());
                }
                error.clear();
                acceptor.close(error); // NOLINT(*-unused-return-value)
                if (error) {
                    LOG_WARNING_TS(Mbs::c_acceptorCloseStatus, error.message());
                }
            }

            { /** Ждем остановки обработчиков в корутинах **/
                Asio::Timer timer { executor };

                while (s_state.load() == State::Shutdown) {
                    timer.expires_after(c_sleepQuantum);
                    co_await timer.async_wait(asio::use_awaitable);
                }
            }
        } catch (const Failure & e) {
            LOG_ERROR_TS(e);
        } catch (const std::exception & e) {
            LOG_ERROR_TS(e);
        } catch (...) {
            LOG_ERROR_TS(Basic::Wcs::c_somethingWrong);
        }

        co_return;
    }

    inline void logError() noexcept {
        switch (s_state.load()) {
            case State::Starting:
                LOG_ERROR_TS(Wcs::c_startingFailed);
                break;
            case State::Stopping:
                LOG_ERROR_TS(Wcs::c_stoppingFailed);
                break;
            default:
                LOG_ERROR_TS(Wcs::c_servicingFailed);
        }
    }

    void run() {
        assert(s_state.load() == State::Initial);
        LOG_DEBUG_TS(Wcs::c_starting);
        s_state.store(State::Starting);

        try {
            {
                Asio::IoContext ioContext(1);
                Asio::SignalSet signals(ioContext, SIGINT, SIGTERM);
                signals.async_wait([] (auto, auto) { std::thread(stop).detach(); });
                s_hitman.placeOrder([&ioContext] { ioContext.stop(); });
                asio::co_spawn(ioContext, listen(), asio::detached);
                ioContext.run();
                s_hitman.cancelOrder();
            }

            LOG_INFO_TS(Wcs::c_stopped);
            s_shutdownSync.arrive_and_wait();
        } catch (...) {
            s_hitman.cancelOrder();
            s_state.store(State::Stopping);
            s_shutdownSync.count_down();
            logError();
            throw;
        }
    }

    void start() try {
        std::thread(run).detach();
    } catch (...) {
        logError();
        throw;
    }

    void stop() try {
        assert(s_state.load() == State::Running);
        LOG_DEBUG_TS(Wcs::c_stopping);

        s_state.store(State::Shutdown);
        s_hitman.await(c_controlTimeout, [] { return Counter::value() > 0; });
        s_state.store(State::Stopping);
        s_hitman.await(c_sleep);
        s_hitman.completeOrder();

        s_shutdownSync.arrive_and_wait();
    } catch (...) {
        logError();
        throw;
    }
}
