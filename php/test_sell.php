#!/usr/bin/php
<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

use Kkm\{ ConnParams, Device, Operations\Sell, Tax, PaymentType, Utils };

// define('OUT_REQUEST_BODY', true);
require_once('lib.php');
$config = require('cfg.php');

$device = new Device(new ConnParams($config['goodNoFiscConnParams']));

$response
    = $device
    ->sell()
    ->setOperator($config['operator'])
    ->setItems([
        ['title' => 'Огурец', 'price' => 234.56, 'quantity' => 1.3],
        ['title' => 'Томат', 'price' => 334.56, 'quantity' => 0.95],
        ['title' => 'Перец', 'price' => 434.56, 'quantity' => 0.3],
    ])->addItem('Сметана', 60.56, 1, Tax::Vat10)
    ->addItem('Молоко', 80.56, 1, 'vat10')
    ->setTax(Tax::Vat20)
    ->setPayment(PaymentType::Cash)
    ->setText('Текст в чеке. Текст в чеке. Текст в чеке. Текст в чеке. Текст в чеке. Текст в чеке. Текст в чеке.')
    ->setHeaderText([
        'content' => 'Здравствуйте!',
        'center' => true,
        'magnified' => true,
    ])
    ->setFooterText('Спасибо!', true, true)
    ->setIdempotencyKey()
    ->perform();

Utils::printResponse($response);

$response
    = $device
    ->sell()
    ->setOperator($config['operator'])
    ->setItems([
        ['title' => 'Товар', 'price' => 100.00, 'quantity' => 1],
    ])->setTax(Tax::Vat5)
    ->setPayment(PaymentType::Cash)
    ->setText('Текст в чеке', true, false, true)
    ->setHeaderText([
        'content' => 'Текст после клише',
        'center' => true,
        'magnified' => true,
        'separated' => true,
    ])
    ->setFooterText('Текст после подвала', true, true, false)
    ->perform(['idempotencyKey' => Utils::uid()]);

Utils::printResponse($response);

$response
    = (new Sell($device))
    ->setIdempotencyKey()
    ->setText('Текст в чеке', true)
    ->perform([
        'operator' => $config['operator'],
        'customer' => $config['customer'],
        'items' => [
            ['title' => 'Дорогой гвоздь', 'price' => 11, 'quantity' => 100, 'tax' => Tax::Vat5],
            ['title' => 'Дорогой молоток', 'price' => 770, 'quantity' => 1, 'tax' => Tax::Vat5],
        ],
        'payment' => ['type' => PaymentType::Electronically],
    ]);

Utils::printResponse($response);

$response
    = (new Sell($device))
    ->setIdempotencyKey()
    ->setText('Текст в чеке', true, true)
    ->perform([
        'operator' => $config['operator'],
        'customer' => $config['customer'],
        'items' => [
            ['title' => 'Дорогой гвоздь', 'price' => 11, 'quantity' => 100, 'tax' => Tax::Vat5],
            ['title' => 'Дорогой молоток', 'price' => 770, 'quantity' => 1, 'tax' => Tax::Vat5],
        ],
        'payment' => ['type' => PaymentType::Electronically],
    ]);

Utils::printResponse($response);

$response
    = (new Sell($device))
    ->setIdempotencyKey()
    ->setText('Текст в чеке', true, false, true)
    ->perform([
        'operator' => $config['operator'],
        'customer' => $config['customer'],
        'items' => [
            ['title' => 'Дорогой гвоздь', 'price' => 11, 'quantity' => 100, 'tax' => Tax::Vat5],
            ['title' => 'Дорогой молоток', 'price' => 770, 'quantity' => 1, 'tax' => Tax::Vat5],
        ],
        'payment' => ['type' => PaymentType::Electronically],
    ]);

Utils::printResponse($response);
