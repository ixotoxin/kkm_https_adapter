<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

namespace Kkm;

class ConnParams
{
    public readonly string $host;
    public readonly string $secret;
    public readonly ?string $serial;

    public function __construct(
        string|array $host,
        ?string $secret = null,
        ?string $serial = null,
    ) {
        if (is_array($host)) {
            $this->host = $host['host'];
            $this->secret = $host['secret'] ?? $secret;
            $this->serial = $host['serial'] ?? $serial;
        } else {
            $this->host = $host;
            $this->secret = $secret;
            $this->serial = $serial;
        }
    }
}
