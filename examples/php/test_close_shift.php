#!/usr/bin/php
<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

use Kkm\{ ConnParams, Device, Utils };

require_once('lib.php');
$config = require('cfg.php');

$device = new Device(new ConnParams($config['goodConnParams']));

$response = $device->closeShift()->setIdempotencyKey()->perform();
Utils::printResponse($response);

$response = $device->closeShift()->setOperator($config['operator'])->setCashOut(true)->setIdempotencyKey()->perform();
Utils::printResponse($response);
