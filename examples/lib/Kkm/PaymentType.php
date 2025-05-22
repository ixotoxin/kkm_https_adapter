<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

namespace Kkm;

enum PaymentType: string
{
    case Cash = 'cash';
    case Electronically = 'electronically';

    static public function cast(mixed $value, bool $nullable = false): ?self
    {
        if ($value instanceof self) {
            return $value;
        }
        if (is_string($value)) {
            return self::from($value);
        }
        if ($nullable && is_null($value)) {
            return null;
        }
        throw new \Exception();
    }
}
