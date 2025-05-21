<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

namespace Kkm;

abstract class DeviceOperation
{
    public function __construct(
        public readonly Device $performer,
    ) {}

    protected function method(): string
    {
        return 'kkm/' . $this->performer->connParams->serial . '/' . static::METHOD;
    }
}
