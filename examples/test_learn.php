#!/usr/bin/php
<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

use Kkm\{ ConnParams, Server, SrvOp, Utils };

require_once('lib.php');
$config = require('cfg.php');

$connParams = new ConnParams($config['goodConnParams']);
$server = new Server($connParams);

$operation = new \Kkm\Operations\Learn($server);
$response = $operation->setConnParams($config['badConnString1'])->setIdempotencyKey()->perform();
Utils::printResponse($response);

$operation = $server->prepare(SrvOp::Learn);
$response = $operation->setConnParams($config['badConnString2'])->setIdempotencyKey()->perform();
Utils::printResponse($response);

$response = $server->prepare(SrvOp::Learn)->setConnParams($config['badConnString3'])->setIdempotencyKey()->perform();
Utils::printResponse($response);

$response = $server->learn()->setConnParams($config['goodConnString'])->setIdempotencyKey()->perform();
Utils::printResponse($response);
