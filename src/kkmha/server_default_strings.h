// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <string_view>

namespace Server::Default::Mbs {
    using Csv = const std::string_view;

    constexpr Csv c_redirectToStaticResponse {
        "HTTP/1.1 302 Moved Temporarily\r\n"
        "Connection: close\r\n"
        "Location: /static/index.html\r\n"
        "Pragma: no-cache\r\n"
        "Cache-Control: no-cache, private\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 128\r\n"
        "\r\n"
        "<script lang=\"javascript\">document.location.href=\"/static/index.html\";</script>"
        "Click this <a href=\"/static/index.html\">link</a>."
    };
}
