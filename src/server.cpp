// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "server.h"
#include <latch>
#include "http_parser.h"
#include "http_request.h"
#include "default_handler.h"
#include "kkm_handler.h"
#include "static_handler.h"
#include "config_handler.h"
#include "ping_handler.h"
#include "server_counter.h"
#include "server_hitman.h"

namespace Server {
    namespace Wcs {
        using namespace Http::Wcs;
    }

    namespace Mbs {
        using namespace Http::Mbs;
    }

    using namespace std::chrono_literals;
    using Basic::Failure;
    using Basic::c_sleep;
    using Basic::c_sleepQuantum;

    static_assert(std::is_integral_v<decltype(c_sleepQuantum)>);
    static_assert(std::is_integral_v<decltype(c_controlTimeout)>);
    static_assert(c_controlTimeout >= c_sleepQuantum);

    enum class State { Initial, Starting, Running, Shutdown, Stopping };

    static std::atomic<State> s_state { State::Initial };
    static Hitman s_hitman {};
    static std::latch s_shutdownSync { 2 };

    static Http::DefaultHandler s_defaultHandler {};
    static Kkm::HttpHandler s_kkmHandler {};
    static Http::StaticHandler s_staticHandler {};
    static Http::ConfigHandler s_configHandler {};
    static Http::PingHandler s_pingHandler {};

    [[nodiscard]]
    inline Http::RequestHandler & lookupHandler(const Http::Request & request) {
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
    auto performAsync(const Http::RequestHandler & handler, Http::Request & request, CompletionToken && token) {
        return
            asio::async_compose<CompletionToken, void()>(
                [& handler, & request] (auto && self) {
                    std::thread(
                        [task = std::move(self), & handler, & request] () mutable {
                            handler(request);
                            task.complete();
                        }
                    ).detach();
                },
                token
            );
    }

    asio::awaitable<void> accept(std::shared_ptr<Counter> counter, auto && socket, Asio::SslContext & sslContext) {
        if (counter->invalid()) {
            tsLogError(Wcs::c_maximumIsExceeded);
            co_return;
        }

        try {
            Asio::Stream stream { std::forward<decltype(socket)>(socket), sslContext };
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
                        tsLogInfo(
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
                    request.authenticate();
                    if (request.m_response.m_status == Http::Status::Ok) {
                        Http::RequestHandler & handler = lookupHandler(request);
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
                        tsLogError(writingError);
                    }
                }

            } catch (const Failure & e) {
                request.m_response.m_status = Http::Status::InternalServerError;
                tsLogError(e);
            } catch (const std::exception & e) {
                request.m_response.m_status = Http::Status::InternalServerError;
                tsLogError(e);
            } catch (...) {
                request.m_response.m_status = Http::Status::InternalServerError;
                tsLogError(Wcs::c_somethingWrong);
            }

            if (request.m_response.m_status < Http::Status::BadRequest) {
                tsLogInfo(Wcs::c_prefixedText, request.m_id, Wcs::c_processingSuccess);
            } else {
                tsLogError(Wcs::c_prefixedText, request.m_id, Wcs::c_processingFailed);
            }

        } catch (const Failure & e) {
            tsLogError(e);
        } catch (const std::exception & e) {
            tsLogError(e);
        } catch (...) {
            tsLogError(Wcs::c_somethingWrong);
        }

        co_return;
    }

    asio::awaitable<void> listen() noexcept {
        assert(s_state.load() == State::Starting);

        try {
            auto port = static_cast<asio::ip::port_type>(s_port);
            Asio::Endpoint endpoint(s_ipv4Only ? asio::ip::tcp::v4() : asio::ip::tcp::v6(), port);
            Asio::SslContext sslContext(Asio::SslContext::tls_server);

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
            sslContext.use_certificate_chain_file(s_certificateChainFile);
            sslContext.use_private_key_file(s_privateKeyFile, Asio::SslContext::pem);
            sslContext.set_verify_mode(asio::ssl::verify_none);

            auto executor = co_await asio::this_coro::executor;
            Asio::Acceptor acceptor(executor, endpoint);

            tsLogInfo(Wcs::c_started);
            s_state.store(State::Running);

            do {
                auto [acceptingError, socket] = co_await acceptor.async_accept();
                if (acceptingError) {
                    tsLogError(acceptingError);
                    tsLogError(Wcs::c_servicingFailed);
                } else if (!socket.is_open()) {
                    tsLogError(Wcs::c_socketOpeningError);
                    tsLogError(Wcs::c_servicingFailed);
                } else {
                    asio::co_spawn(
                        executor,
                        accept(std::make_shared<Counter>(), std::move(socket), sslContext),
                        asio::detached
                    );
                }
            } while (s_state.load() == State::Running);

            Asio::Timer timer(executor);

            while (s_state.load() == State::Shutdown) {
                timer.expires_after(std::chrono::milliseconds(c_sleepQuantum));
                co_await timer.async_wait(asio::use_awaitable);
            }
        } catch (const Failure & e) {
            tsLogError(e);
        } catch (const std::exception & e) {
            tsLogError(e);
        } catch (...) {
            tsLogError(Wcs::c_somethingWrong);
        }

        co_return;
    }

    inline void logError() noexcept {
        switch (s_state.load()) {
            case State::Starting:
                tsLogError(Wcs::c_startingFailed);
                break;
            case State::Stopping:
                tsLogError(Wcs::c_stoppingFailed);
                break;
            default:
                tsLogError(Wcs::c_servicingFailed);
        }
    }

    void run() {
        assert(s_state.load() == State::Initial);
        // if (s_state.load() != State::Initial) {
        //     return;
        // }

        tsLogDebug(Wcs::c_starting);
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

            tsLogInfo(Wcs::c_stopped);
            s_shutdownSync.arrive_and_wait();
        } catch (...) {
            logError();
            s_hitman.cancelOrder();
            s_state.store(State::Stopping);
            s_shutdownSync.count_down();
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
        // if (s_state.load() != State::Running) {
        //     return;
        // }

        tsLogDebug(Wcs::c_stopping);
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
