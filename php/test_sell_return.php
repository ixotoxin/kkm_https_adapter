#!/usr/bin/php
<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

use Kkm\{ ConnParams, Device, Operations\SellReturn, Tax, PaymentType, Utils };

// define('OUT_REQUEST_BODY', true);
require_once('lib.php');
$config = require('cfg.php');

$device = new Device(new ConnParams($config['goodNoFiscConnParams']));

$response
    = $device
    ->sellReturn()
    ->setOperator($config['operator'])
    ->setItems([
        ['title' => 'Товар', 'price' => 100.00, 'quantity' => 1],
    ])->setTax(Tax::Vat5)
    ->setPayment(PaymentType::Cash)
    ->setText('Текст в чеке')
    ->setHeaderText([
        'content' => 'Текст после клише',
        'center' => true,
        'magnified' => true,
    ])
    ->setFooterText('Спасибо!', true, true)
    ->setIdempotencyKey()
    ->perform();

Utils::printResponse($response);

$response
    = (new SellReturn($device))
    ->perform([
        'idempotencyKey' => Utils::uid(),
        'operator' => $config['operator'],
        'customer' => $config['customer'],
        'items' => [
            ['title' => 'Дорогой гвоздь', 'price' => 11, 'quantity' => 50, 'tax' => Tax::Vat5],
            ['title' => 'Дорогой молоток', 'price' => 770, 'quantity' => 0.5, 'tax' => Tax::Vat5],
        ],
        'payment' => ['type' => PaymentType::Electronically],
        'text' => ['content' => 'Просто текст'],
        'headerText' => ['content' => 'Текст после клише'],
        'footerText' => ['content' => 'Текст после подвала'],
    ]);

Utils::printResponse($response);
