<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

namespace Kkm\Operations;

defined('KKMHA_EXAMPLES') or die('No direct script access.');

final class BaseStatus extends \Kkm\DeviceOperation implements \Kkm\GetOperation
{
    use \Kkm\GetOperationTrait;

    public const METHOD = 'base-status';
}
