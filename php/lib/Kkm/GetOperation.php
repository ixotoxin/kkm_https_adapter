<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

namespace Kkm;

interface GetOperation
{
    public function setIdempotencyKey(string $key): static;
    public function perform(): array;
}
