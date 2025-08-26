#!/usr/bin/php
<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

use Kkm\{ ConnParams, Device, DevOp, Operations, Utils };

require_once('lib.php');
$config = require('cfg.php');

$response = (new Device(new ConnParams($config['badSecretConnParams'])))->baseStatus()->perform();
Utils::printResponse($response);

$response = (new Device(new ConnParams($config['badSerialConnParams'])))->prepare(DevOp::BaseStatus)->perform();
Utils::printResponse($response);

$response = (new Operations\BaseStatus(new Device(new ConnParams($config['goodConnParams']))))->perform();
Utils::printResponse($response);
