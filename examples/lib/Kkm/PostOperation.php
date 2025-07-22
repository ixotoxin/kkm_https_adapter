<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

namespace Kkm;

defined('KKMHA_EXAMPLES') or die('No direct script access.');

interface PostOperation
{
    public function setIdempotencyKey(string $key): static;
    public function setPadding(string $padding): static;
    public function setDetails(array $details): static;
    public function perform(array $details = []): array;
}
