#!/usr/bin/php
<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

use Kkm\{ Utils, ConnParams, Server, Operations\ResetRegistry };

require_once('lib.php');
$config = require('cfg.php');

$operation = new ResetRegistry(new Server(new ConnParams($config['goodConnParams'])));

$string1 = 'X';
$string2 = str_repeat('Y', 50000);
$string3 = str_repeat('Z', 100000);
$i = 100;
while (--$i) {
    $j = 10;
    while (--$j) {
        Utils::printResponseSummary($operation->setIdempotencyKey(Utils::uid())->setPadding($string1)->perform());
        $string1 .= random_int(0, 9);
    }
    $j = 10;
    while (--$j) {
        Utils::printResponseSummary($operation->setIdempotencyKey(Utils::uid())->setPadding($string2)->perform());
        $string2 .= str_repeat((string)random_int(0, 9), random_int(2, 20));
    }
    $j = 10;
    while (--$j) {
        Utils::printResponseSummary($operation->setIdempotencyKey(Utils::uid())->setPadding($string3)->perform());
        $string3 .= str_repeat((string)random_int(0, 9), random_int(20, 200));
    }
}
