// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "library/basic.h"
#include "defaults.h"

namespace Server {
    void run();
    void start();
    void stop();

    inline bool s_ipv4Only { c_defIpv4Only };
    inline unsigned short s_port { c_defPort };
    inline int64_t s_concurrencyLimit { c_defConcurrencyLimit };
    inline bool s_enableLegacyTls { false };
    inline int s_securityLevel { -1 };
    inline std::string s_certificateChainFile { c_defCertificateChainFile };
    inline std::string s_privateKeyFile { c_defPrivateKeyFile };
    inline std::string s_privateKeyPassword {};
}
