<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

return [
    'goodConnParams' => [
        'host' => '127.11.22.33:5757',
        'secret' => 'lorem.ipsum.dolor.sit.amet',
        'serial' => '11111111111111',
    ],
    'goodNoFiscConnParams' => [
        'host' => '127.11.22.33:5757',
        'secret' => 'lorem.ipsum.dolor.sit.amet',
        'serial' => '22222222222222',
    ],
    'badConnParams' => [
        'host' => '127.0.0.1:5757',
        'secret' => 'lorem.ipsum.dolor.sit.amet',
        'serial' => '33333333333333',
    ],
    'badSecretConnParams' => [
        'host' => '127.0.0.1:5757',
        'secret' => '!.BAD.SECRET.!',
        'serial' => '22222222222222',
    ],
    'badSerialConnParams' => [
        'host' => '127.0.0.1:5757',
        'secret' => 'lorem.ipsum.dolor.sit.amet',
        'serial' => '22222222222222',
    ],
    'goodConnString' => 'com,4',
    'badConnString1' => 'com,7',
    'badConnString2' => 'tcpip,172.0.0.1',
    'badConnString3' => 'abra,cad,abra',
    'operator' => [
        'name' => 'Оператрисса',
        'inn' => '770000000000',
    ],
    'customer' => [
        'account' => '123456789',
        'contact' => '+79876543210',
        'name' => 'Покупатель Товаров',
    ],
];
