<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

namespace Kkm;

trait SetCashSumTrait
{
    public function setCashSum(int|float $sum): static
    {
        if ($sum < 0.01 || $sum > 999999.99) {
            throw new \Exception();
        }
        $this->details['cashSum'] = $sum;
        return $this;
    }
}
