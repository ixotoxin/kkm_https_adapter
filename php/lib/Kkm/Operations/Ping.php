<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

namespace Kkm\Operations;

final class Ping extends \Kkm\ServerOperation implements \Kkm\GetOperation
{
    use \Kkm\GetOperationTrait;

    public const METHOD = 'ping';
}
