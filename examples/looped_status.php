#!/usr/bin/php
<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

use Kkm\{ Utils, ConnParams, Device, Operations };

require_once('lib.php');
$config = require('cfg.php');

$goodOperation1 = new Operations\Status(new Device(new ConnParams($config['goodConnParams'])));
$goodOperation2 = new Operations\Status(new Device(new ConnParams($config['goodConnParams'])));
$badOperation = new Operations\Status(new Device(new ConnParams($config['badConnParams'])));
$badSecretOperation = new Operations\Status(new Device(new ConnParams($config['badSecretConnParams'])));
$badSerialOperation = new Operations\Status(new Device(new ConnParams($config['badSerialConnParams'])));

$i = 9;
while (true) {
    if ($i === 5) {
        Utils::printResponseSummary($goodOperation1->setIdempotencyKey(Utils::uid())->perform());
        Utils::printResponseSummary($goodOperation1->perform());
        --$i;
    } elseif ($i) {
        Utils::printResponseSummary($goodOperation2->perform());
        --$i;
    } else {
        Utils::printResponseSummary($badOperation->perform());
        Utils::printResponseSummary($badSecretOperation->perform());
        Utils::printResponseSummary($badSerialOperation->perform());
        $i = 9;
    }
}
