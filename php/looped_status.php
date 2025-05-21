#!/usr/bin/php
<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

use Kkm\{ Utils, ConnParams, Device, Operations\Status };

require_once('lib.php');
$config = require('cfg.php');

$goodOperation1 = new Status(new Device(new ConnParams($config['goodConnParams'])));
$goodOperation2 = new Status(new Device(new ConnParams($config['goodConnParams'])));
$badOperation = new Status(new Device(new ConnParams($config['badConnParams'])));
$badSecretOperation = new Status(new Device(new ConnParams($config['badSecretConnParams'])));
$badSerialOperation = new Status(new Device(new ConnParams($config['badSerialConnParams'])));

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
