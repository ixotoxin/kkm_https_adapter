#!/usr/bin/php
<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

use Kkm\{ ConnParams, Device, DevOp, Utils };

require_once('lib.php');
$config = require('cfg.php');

$response = (new Device(new ConnParams($config['badSecretConnParams'])))->status()->perform();
Utils::printResponse($response);

$response = (new Device(new ConnParams($config['badSerialConnParams'])))->prepare(DevOp::Status)->perform();
Utils::printResponse($response);

$response = (new \Kkm\Operations\Status(new Device(new ConnParams($config['goodConnParams']))))->perform();
Utils::printResponse($response);
