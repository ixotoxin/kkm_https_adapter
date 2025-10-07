<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

namespace Kkm;

defined('KKMHA_EXAMPLES') or die('No direct script access.');

trait SetCashSumTrait
{
    public function setCashSum(int|float $sum): static
    {
        if ($sum < 0.01 || $sum > 999999.99) {
            throw new \RuntimeException();
        }
        $this->details['cashSum'] = $sum;
        return $this;
    }
}
