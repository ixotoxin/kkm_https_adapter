<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

namespace Kkm;

defined('KKMHA_EXAMPLES') or die('No direct script access.');

trait SetIdempotencyKeyTrait
{
    protected ?string $idempotencyKey = null;

    public function setIdempotencyKey(?string $key = null): static
    {
        $this->idempotencyKey = $key ?? Utils::uid();
        return $this;
    }
}
