<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

namespace Kkm;

defined('KKMHA_EXAMPLES') or die('No direct script access.');

class Utils
{
    static function nullableString(mixed $value): ?string
    {
        if (is_string($value) || is_null($value)) {
            return $value;
        }
        throw new \Exception();
    }

    static function uid()
    {
        return bin2hex(random_bytes(32));
    }

    static function printResponse(array $response): void
    {
        echo '<pre>', PHP_EOL;
        var_export($response);
        echo PHP_EOL, '</pre>';
    }

    static function filterResponse(array $response): array
    {
        return array_filter(
            $response,
            fn ($v, $k) => str_starts_with($k, '!'),
            ARRAY_FILTER_USE_BOTH
        );
    }

    static function printResponseSummary(array $response): void
    {
        echo implode(' | ', static::filterResponse($response)), PHP_EOL;
    }
}
