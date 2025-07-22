<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

namespace Kkm\Operations;

defined('KKMHA_EXAMPLES') or die('No direct script access.');

final class PrintFnRegistrations extends \Kkm\DeviceOperation implements \Kkm\PostOperation
{
    use \Kkm\PostOperationTrait;

    public const METHOD = 'print-fn-registrations';
}
