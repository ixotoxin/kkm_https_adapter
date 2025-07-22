#!/usr/bin/php
<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

use Kkm\{ ConnParams, Device, Utils };

require_once('lib.php');
$config = require('cfg.php');

$device = new Device(new ConnParams($config['goodConnParams']));

$response = $device->cashIn()->setOperator($config['operator'])->setCashSum(3500)->setIdempotencyKey()->perform();
Utils::printResponse($response);
