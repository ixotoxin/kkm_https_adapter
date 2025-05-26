// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "server.h"
#include <array>
#include "http_parser.h"
// #include "http_cache.h"
#include "default_handler.h"
#include "kkm_handler.h"
#include "static_handler.h"
#include "config_handler.h"
#include "ping_handler.h"

namespace Server {
    namespace Wcs {
        using namespace Http::Wcs;
    }

    namespace Mbs {
        using namespace Http::Mbs;
    }

    using Basic::Failure;

    static std::atomic<bool> s_started { false };
    static std::atomic<bool> s_running { true };
    static std::shared_ptr<Asio::IoContext> s_ioContext { nullptr };

    static const std::array<std::shared_ptr<Http::RequestHandler>, 5> s_handlers {
        std::make_shared<Http::DefaultHandler>(),
        std::make_shared<Kkm::HttpHandler>(),
        std::make_shared<Http::StaticHandler>(),
        std::make_shared<Http::ConfigHandler>(),
        std::make_shared<Http::PingHandler>()
    };

    [[nodiscard]]
    inline std::shared_ptr<Http::RequestHandler> lookupHandler(const Http::Request & request) {
        // ISSUE: При большем количестве обработчиков стоит оптимизировать.
        if (request.m_hint.size() >= 2) {
            if (request.m_hint[1] == "kkm") {
                return s_handlers[1];
            } else if (request.m_hint[1] == "static") {
                return s_handlers[2];
            } else if (request.m_hint[1] == "config") {
                return s_handlers[3];
            } else if (request.m_hint[1] == "ping") {
                return s_handlers[4];
            }
        }
        return s_handlers[0];
    }

    template<typename CompletionToken>
    auto performAsync(const Http::RequestHandler & handler, Http::Request & request, CompletionToken && token) {
        return
            asio::async_compose<CompletionToken, void()>(
                [& handler, & request] (auto && self) {
                    std::thread thread(
                        [task = std::move(self), & handler, & request] () mutable {
                            handler(request);
                            task.complete();
                        }
                    );
                    thread.detach();
                },
                token
            );
    }

    asio::awaitable<void> accept(auto && socket, Asio::SslContext & sslContext) {
        Asio::Stream stream { std::forward<decltype(socket)>(socket), sslContext };
        Http::Request request { stream.lowest_layer().remote_endpoint().address() };

        try {
            { /** Приветствуем **/
                const auto & [handshakeError] = co_await stream.async_handshake(asio::ssl::stream_base::server);
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
                Http::authenticate(request);
                if (request.m_response.m_status == Http::Status::Ok) {
                    auto handler = lookupHandler(request);
                    if (handler) {
                        /*if (handler->isCaching()) {
                            Http::Cache::maintain();
                        }*/
                        if (handler->asyncReady()) {
                            co_await performAsync(*handler, request, asio::use_awaitable);
                        } else {
                            (*handler)(request);
                        }
                    } else {
                        request.fail(Http::Status::MethodNotAllowed, Mbs::c_methodNotAllowed);
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
        co_return; // ???
    }

    asio::awaitable<void> listen() {
        bool error = true;

        try {
            auto port = static_cast<asio::ip::port_type>(s_port);
            Asio::Endpoint endpoint(s_ipv4Only ? asio::ip::tcp::v4() : asio::ip::tcp::v6(), port);

            // ISSUE: А есть ли в этом необходимость? {{{
            // bool failed { false };
            // defaultProvider = OSSL_PROVIDER_load(nullptr, "default");
            // if (s_enableLegacyTls) {
            //     legacyProvider = OSSL_PROVIDER_load(nullptr, "legacy");
            // }
            // if (!defaultProvider || !::OSSL_PROVIDER_available(nullptr, "default")) {
            //     tsLogError(Wcs::c_osslProviderNotLoaded, L"default");
            //     failed = true;
            // }
            // if (s_enableLegacyTls && (!legacyProvider || !::OSSL_PROVIDER_available(nullptr, "legacy"))) {
            //     tsLogError(Wcs::c_osslProviderNotLoaded, L"legacy");
            //     failed = true;
            // }
            // if (failed) {
            //     throw Failure(Wcs::c_osslInitializationFailed); // NOLINT(*-exception-baseclass)
            // }
            // }}}

            Asio::SslContext sslContext(Asio::SslContext::tls_server);

            constexpr auto alwaysClear
                = Asio::SslContext::no_tlsv1_2 | Asio::SslContext::no_tlsv1_3;

            constexpr auto alwaysSet
                = Asio::SslContext::default_workarounds | Asio::SslContext::no_sslv2 | Asio::SslContext::no_sslv3;

            if (s_enableLegacyTls) {
                sslContext.clear_options(alwaysClear | Asio::SslContext::no_tlsv1 | Asio::SslContext::no_tlsv1_1);
                sslContext.set_options(alwaysSet);
                // ISSUE: А есть ли в этом необходимость? {{{
                ::SSL_CTX_clear_options(
                    sslContext.native_handle(),
                    SSL_OP_NO_TLSv1 | SSL_OP_NO_TLSv1_1 | SSL_OP_NO_TLSv1_2 | SSL_OP_NO_TLSv1_3
                );
                ::SSL_CTX_set_options(
                    sslContext.native_handle(),
                    SSL_OP_ALLOW_UNSAFE_LEGACY_RENEGOTIATION | SSL_OP_LEGACY_SERVER_CONNECT
                    | SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3
                );
                ::SSL_CTX_set_cipher_list(sslContext.native_handle(), "ALL:COMPLEMENTOFALL");
                // ::SSL_CTX_set_cipher_list(sslContext.native_handle(), "ALL:COMPLEMENTOFALL:TLSv1.2:TLSv1.0");
                // ::SSL_CTX_set_cipher_list(sslContext.native_handle(), "ALL:COMPLEMENTOFALL:TLSv1.2:TLSv1.0:SSLv3");
                // }}}
                ::SSL_CTX_set_security_level(sslContext.native_handle(), 0);
                ::SSL_CTX_set_min_proto_version(sslContext.native_handle(), TLS1_VERSION);
            } else {
                sslContext.clear_options(alwaysClear);
                sslContext.set_options(alwaysSet | Asio::SslContext::no_tlsv1 | Asio::SslContext::no_tlsv1_1);
                if (s_securityLevel >= 0) {
                    ::SSL_CTX_set_security_level(sslContext.native_handle(), s_securityLevel);
                }
            }

            sslContext.set_password_callback([] (auto, auto) { return s_privateKeyPassword; });
            sslContext.use_certificate_chain_file(s_certificateChainFile);
            sslContext.use_private_key_file(s_privateKeyFile, Asio::SslContext::pem);

            auto executor = co_await asio::this_coro::executor;
            Asio::Acceptor acceptor(executor, endpoint);

            s_started = true;
            tsLogInfo(Wcs::c_started);

            while (s_running) {
                auto [acceptingError, socket] = co_await acceptor.async_accept();
                if (!acceptingError && socket.is_open()) {
                    co_spawn(executor, accept(std::move(socket), sslContext), asio::detached);
                } else {
                    tsLogError(acceptingError);
                    tsLogError(Wcs::c_servicingFailed);
                }
            }

            error = false;
        } catch (const Failure & e) {
            tsLogError(e);
        } catch (const std::exception & e) {
            tsLogError(e);
        } catch (...) {
            tsLogError(Wcs::c_somethingWrong);
        }

        if (error && s_ioContext) {
            tsLogDebug(Wcs::c_stopping);
            s_ioContext->stop();
        }
        co_return; // ???
    }

    void run() {
        bool error = true;

        try {
            tsLogDebug(Wcs::c_starting);

            s_ioContext = std::make_shared<Asio::IoContext>(1);
            asio::signal_set signals(*s_ioContext, SIGINT, SIGTERM);

            signals.async_wait(
                [ioContext = s_ioContext] (auto, auto) {
                    tsLogDebug(Wcs::c_stopping);
                    ioContext->stop();
                }
            );

            co_spawn(*s_ioContext, listen(), asio::detached);
            s_ioContext->run();
            error = false;
        } catch (const Failure & e) {
            tsLogError(e);
        } catch (const std::exception & e) {
            tsLogError(e);
        } catch (...) {
            tsLogError(Wcs::c_somethingWrong);
        }

        if (error) {
            if (s_started) {
                tsLogError(Wcs::c_servicingFailed);
            } else {
                tsLogError(Wcs::c_startingFailed);
            }
        } else {
            // ISSUE: А есть ли в этом необходимость? {{{
            // if (legacyProvider) {
            //     ::OSSL_PROVIDER_unload(legacyProvider);
            // }
            // if (defaultProvider) {
            //     ::OSSL_PROVIDER_unload(defaultProvider);
            // }
            // }}}
            tsLogInfo(Wcs::c_stopped);
        }
    }

    void start() {
        std::thread thread([] { run(); });
        thread.detach();
    }

    void stop() {
        bool error = true;

        try {
            tsLogDebug(Wcs::c_stopping);
            s_running = false;
            ::Sleep(500);
            if (s_ioContext) {
                s_ioContext->stop();
            }
            error = false;
        } catch (const Failure & e) {
            tsLogError(e);
        } catch (const std::exception & e) {
            tsLogError(e);
        } catch (...) {
            tsLogError(Wcs::c_somethingWrong);
        }

        if (error) {
            if (s_started) {
                tsLogError(Wcs::c_stoppingFailed);
            } else {
                tsLogError(Wcs::c_servicingFailed);
            }
        } else {
            tsLogInfo(Wcs::c_stopped);
        }
    }
}
