// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <asio.hpp>
#include <asio/ssl.hpp>

namespace Asio {
    using Error = asio::error_code;
    using Timer = asio::steady_timer;
    using IpAddress = asio::ip::address;
    using IoContext = asio::io_context;
    using SignalSet = asio::signal_set;
    using StreamBuffer = asio::streambuf;
    using SslContext = asio::ssl::context;
    using DefaultToken = asio::as_tuple_t<asio::use_awaitable_t<>>;
    using TcpAcceptor = asio::ip::tcp::acceptor;
    using Acceptor = DefaultToken::as_default_on_t<TcpAcceptor>;
    using TcpSocket = asio::ip::tcp::socket;
    using Socket = DefaultToken::as_default_on_t<TcpSocket>;
    using Stream = asio::ssl::stream<Socket>;
    using Endpoint = asio::ip::tcp::endpoint;

    [[nodiscard, maybe_unused]]
    inline bool isLoopback(const IpAddress & address) {
        if (address.is_v4()) {
            return address.to_v4().is_loopback();
        } else {
            auto v6 = address.to_v6();
            if (v6.is_v4_mapped() || v6.is_v4_compatible()) {
                return v6.to_v4().is_loopback();
            }
            return v6.is_loopback();
        }
    }
}
