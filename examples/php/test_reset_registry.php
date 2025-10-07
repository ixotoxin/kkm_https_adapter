#!/usr/bin/php
<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

use Kkm\{ ConnParams, Server, Utils };

require_once('lib.php');
$config = require('cfg.php');

$server = new Server(new ConnParams($config['goodConnParams']));

$response = $server->resetRegistry()->setIdempotencyKey()->perform();
Utils::printResponse($response);
