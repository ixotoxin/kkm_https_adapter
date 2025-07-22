<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

namespace Kkm;

defined('KKMHA_EXAMPLES') or die('No direct script access.');

enum Tax: string
{
    case No = 'no';
    case Vat0 = 'vat0';
    case Vat5 = 'vat5';
    case Vat105 = 'vat105';
    case Vat7 = 'vat7';
    case Vat107 = 'vat107';
    case Vat10 = 'vat10';
    case Vat110 = 'vat110';
    case Vat20 = 'vat20';
    case Vat120 = 'vat120';

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
        throw new \LogicException();
    }
}
