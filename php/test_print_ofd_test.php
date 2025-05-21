#!/usr/bin/php
<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

use Kkm\{ ConnParams, Device, Utils };

require_once('lib.php');
$config = require('cfg.php');

$connParams = new ConnParams($config['goodConnParams']);
$device = new Device($connParams);

$response = $device->printOfdTest()->setIdempotencyKey()->perform();
Utils::printResponse($response);
