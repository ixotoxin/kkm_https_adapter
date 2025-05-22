#!/usr/bin/php
<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

use Kkm\{ Utils, ConnParams, Server, Operations\Ping };

require_once('lib.php');
$config = require('cfg.php');

$operation = new Ping(new Server(new ConnParams($config['goodConnParams'])));

while (true) {
    Utils::printResponseSummary($operation->perform());
}
