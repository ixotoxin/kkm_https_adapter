<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

namespace Kkm;

defined('KKMHA_EXAMPLES') or die('No direct script access.');

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
            $this->secret = $host['secret'] ?? '';
            $this->serial = $host['serial'] ?? '';
        } else {
            $this->host = $host;
            $this->secret = $secret ?? '';
            $this->serial = $serial ?? '';
        }
    }
}
