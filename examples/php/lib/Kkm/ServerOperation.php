<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

namespace Kkm;

defined('KKMHA_EXAMPLES') or die('No direct script access.');

abstract class ServerOperation
{
    public function __construct(
        public readonly Server $performer,
    ) {}

    protected function method(): string
    {
        return static::METHOD;
    }
}
