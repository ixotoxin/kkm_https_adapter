#!/usr/bin/php
<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

use Kkm\{ ConnParams, Device, Operations, Utils };

require_once('lib.php');
$config = require('cfg.php');

$response
    = (new Operations\ReportX(new Device(new ConnParams($config['goodConnParams']))))
    ->setOperator($config['operator'])
    ->setIdempotencyKey()
    ->perform();

Utils::printResponse($response);
