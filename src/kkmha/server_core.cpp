// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "server_core.h"
#include "server_variables.h"
#include "server_strings.h"
#include "server_failure.h"
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
#include <chrono>

namespace Server {
    using namespace std::chrono_literals;

    static_assert(c_controlTimeout >= c_sleepQuantum);

    enum class State { Initial, Starting, Running, Shutdown, Stopping };

    static std::atomic s_state { State::Initial };
    static Counter::Type s_concurrentRequestsCounter { 0 };
    static Counter::Type s_delayedSocketsCounter { 0 };
    static Hitman s_hitman {};
    static std::latch s_shutdownSync { 2 };

    static Default::Handler s_defaultHandler {};
    static KkmOp::Handler s_kkmHandler {};
    static Static::Handler s_staticHandler {};
    static Config::Handler s_configHandler {};
    static Ping::Handler s_pingHandler {};

    [[nodiscard]]
    /*inline*/ ProtoHandler & lookupHandler(const Http::Request & request) {
        // ISSUE: При большем количестве обработчиков стоит оптимизировать.
        if (request.m_hint.size() >= 2) {
            const auto area = request.m_hint[1];
            if (area == "kkm") {
                return s_kkmHandler;
            }
            if (area == "static") {
                return s_staticHandler;
            }
            if (area == "config") {
                return s_configHandler;
            }
            if (area == "ping") {
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
        Counter counter { s_concurrentRequestsCounter };

        try {
            Asio::Stream stream { std::forward<Asio::TcpSocket>(socket), sslContext };
            Http::Request request { stream.lowest_layer().remote_endpoint().address() };
            auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(s_requestTimeout);
            Asio::Timer timeoutTimer { co_await asio::this_coro::executor };
            Asio::Error error {};
            Asio::CancellationSignal signal {};
            bool canceled { false };

            timeoutTimer.expires_at(deadline);
            timeoutTimer.async_wait(
                [& signal, & canceled]
                (Asio::Error error) {
                    if (!error) {
                        canceled = true;
                        signal.emit(asio::cancellation_type::all);
                    }
                }
            );

            try {
                {
                    co_await stream.async_handshake(
                        asio::ssl::stream_base::server,
                        asio::bind_cancellation_slot(
                            signal.slot(),
                            asio::redirect_error(asio::use_awaitable, error)
                        )
                    );
                    if (error) {
                        throw Failure(request.m_id, Mbs::c_sslHandshakeOperation, error); // NOLINT(*-exception-baseclass)
                    }
                }

                if (!canceled) {
                    Asio::StreamBuffer buffer {};

                    co_await asio::async_read_until(
                        stream, buffer,
                        "\r\n\r\n",
                        asio::bind_cancellation_slot(
                            signal.slot(),
                            asio::redirect_error(asio::use_awaitable, error)
                        )
                    );
                    if (error) {
                        throw Failure(request.m_id, Mbs::c_sslReadOperation, error); // NOLINT(*-exception-baseclass)
                    }

                    Http::Parser parser(request);
                    parser(buffer);

                    if (request.m_method == Http::Method::Post) {
                        auto expecting = parser.expecting();
                        while (!canceled && expecting) {
                            co_await asio::async_read(
                                stream, buffer,
                                asio::transfer_at_least(expecting),
                                asio::bind_cancellation_slot(
                                    signal.slot(),
                                    asio::redirect_error(asio::use_awaitable, error)
                                )
                            );
                            if (error) {
                                throw Failure(request.m_id, Mbs::c_sslReadOperation, error); // NOLINT(*-exception-baseclass)
                            }
                            parser(buffer);
                            expecting = parser.expecting();
                        }
                    }
                    parser.complete();
                }

                if (!canceled) {
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

                if (!canceled) {
                    Asio::StreamBuffer buffer {};
                    request.m_response.render(buffer);
                    co_await asio::async_write(
                        stream, buffer,
                        asio::bind_cancellation_slot(
                            signal.slot(),
                            asio::redirect_error(asio::use_awaitable, error)
                        )
                    );
                    if (error) {
                        throw Failure(request.m_id, Mbs::c_sslWriteOperation, error); // NOLINT(*-exception-baseclass)
                    }
                }

            } catch (const Basic::Failure & e) {
                if (!canceled) {
                    request.m_response.m_status = Http::Status::InternalServerError;
                    LOG_ERROR_TS(e);
                }
            } catch (const std::exception & e) {
                if (!canceled) {
                    request.m_response.m_status = Http::Status::InternalServerError;
                    LOG_ERROR_TS(e);
                }
            } catch (...) {
                if (!canceled) {
                    request.m_response.m_status = Http::Status::InternalServerError;
                    LOG_ERROR_TS(Basic::Wcs::c_somethingWrong);
                }
            }

            timeoutTimer.cancel();

            {
                stream.lowest_layer().cancel();

                if (stream.lowest_layer().is_open()) {
                    error.clear();
                    stream.shutdown(error); // NOLINT(*-unused-return-value)
                    if (error) {
                        if (error.category() == asio::error::get_ssl_category()) {
                            auto sslError = ERR_GET_REASON(error.value());
                            if (sslError == SSL_R_APPLICATION_DATA_AFTER_CLOSE_NOTIFY) {
                                error = asio::ssl::error::stream_truncated;
                            }
                        }
                        if (
                            error != asio::ssl::error::stream_truncated // <= обязательно игнорируем
                            && error != asio::error::eof
                            && error != asio::error::connection_aborted // <= обязательно игнорируем
                            && error != asio::error::connection_refused
                            && error != asio::error::connection_reset   // <= обязательно игнорируем
                            && error != asio::error::not_connected
                        ) {
                            if (Log::s_appendLocation) {
                                LOG_ERROR_TS(
                                    Mbs::c_prefixedOperationWithSource, request.m_id, Mbs::c_sslShutdownOperation,
                                    error.message(), SrcLoc::toMbs(SrcLoc::Point::current())
                                );
                            } else {
                                LOG_ERROR_TS(
                                    Mbs::c_prefixedOperation, request.m_id, Mbs::c_sslShutdownOperation, error.message()
                                );
                            }
                        }
                    }
                }

                if (stream.lowest_layer().is_open()) {
                    error.clear();
                    stream.lowest_layer().close(error); // NOLINT(*-unused-return-value)
                    if (error) {
                        if (Log::s_appendLocation) {
                            LOG_ERROR_TS(
                                Mbs::c_prefixedOperationWithSource, request.m_id, Mbs::c_socketCloseOperation,
                                error.message(), SrcLoc::toMbs(SrcLoc::Point::current())
                            );
                        } else {
                            LOG_ERROR_TS(
                                Mbs::c_prefixedOperation, request.m_id, Mbs::c_socketCloseOperation, error.message()
                            );
                        }
                    }
                }
            }

            if (canceled) {
                LOG_WARNING_TS(Wcs::c_prefixedText, request.m_id, Wcs::c_timeoutExpired);
            } else if (request.m_response.m_status < Http::Status::BadRequest) {
                LOG_INFO_TS(Wcs::c_prefixedText, request.m_id, Wcs::c_processingSuccess);
            } else {
                LOG_WARNING_TS(Wcs::c_prefixedText, request.m_id, Wcs::c_processingFailed);
            }

        } catch (const Basic::Failure & e) {
            LOG_ERROR_TS(e);
        } catch (const std::exception & e) {
            LOG_ERROR_TS(e);
        } catch (...) {
            LOG_ERROR_TS(Basic::Wcs::c_somethingWrong);
        }

        co_return;
    }

    asio::awaitable<void> close(Asio::TcpSocket && socket0) {
        try {
            Counter counter { s_delayedSocketsCounter };
            Asio::TcpSocket socket { std::forward<Asio::TcpSocket>(socket0) }; // Чтобы сокет пережил co_await
            Asio::Timer timer { co_await asio::this_coro::executor };

            timer.expires_after(c_closingDelay);
            co_await timer.async_wait(asio::use_awaitable);
            socket.cancel();
            socket.shutdown(Asio::TcpSocket::shutdown_both);
            socket.close();
        } catch (...) {}
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

            {
                auto executor = co_await asio::this_coro::executor;
                Asio::Acceptor acceptor { executor, endpoint };

                LOG_INFO_TS(Wcs::c_started);
                s_state.store(State::Running);

                do {
                    auto [error, socket] = co_await acceptor.async_accept();
                    if (error) {
                        LOG_ERROR_TS(Mbs::c_connectionAcceptStatus, error.message());
                        LOG_ERROR_TS(Wcs::c_servicingFailed);
                    } else if (!socket.is_open()) {
                        LOG_ERROR_TS(Wcs::c_socketOpeningError);
                        LOG_ERROR_TS(Wcs::c_servicingFailed);
                    } else if (s_concurrentRequestsCounter >= s_concurrencyLimit) {
                        if (s_delayedSocketsCounter >= c_delayedSockets) {
                            socket.cancel();
                            socket.shutdown(Asio::TcpSocket::shutdown_both);
                            socket.close();
                        } else {
                            LOG_ERROR_TS(Wcs::c_maximumIsExceeded);
                            asio::co_spawn(executor, close(std::move(socket)), asio::detached);
                        }
                    } else {
                        asio::co_spawn(executor, accept(std::move(socket), sslContext), asio::detached);
                    }
                } while (s_state.load() == State::Running);

                {
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
            }

            {
                Asio::Timer timer { co_await asio::this_coro::executor };

                while (s_state.load() == State::Shutdown) {
                    timer.expires_after(c_sleepQuantum);
                    co_await timer.async_wait(asio::use_awaitable);
                }
            }
        } catch (const Basic::Failure & e) {
            LOG_ERROR_TS(e);
        } catch (const std::exception & e) {
            LOG_ERROR_TS(e);
        } catch (...) {
            LOG_ERROR_TS(Basic::Wcs::c_somethingWrong);
        }

        co_return;
    }

    /*inline*/ void logError() noexcept {
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
                Asio::IoContext ioContext { std::clamp(static_cast<int>(std::thread::hardware_concurrency()), 1, 4) };
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
        s_hitman.await(c_controlTimeout, [] { return s_concurrentRequestsCounter > 0; });
        s_state.store(State::Stopping);
        s_hitman.await(c_sleep);
        s_hitman.completeOrder();

        s_shutdownSync.arrive_and_wait();
    } catch (...) {
        logError();
        throw;
    }
}
