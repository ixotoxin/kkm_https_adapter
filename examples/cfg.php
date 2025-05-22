<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

return [
    'goodConnParams' => [
        'host' => '127.0.22.33:5757',
        'secret' => 'lorem.ipsum.dolor.sit.amet',
        // 'serial' => '00106308755095',
        'serial' => '00106308750663',
    ],
    'goodNoFiscConnParams' => [
        'host' => '127.0.22.33:5757',
        'secret' => 'lorem.ipsum.dolor.sit.amet',
        'serial' => '00106308755095',
    ],
    'badConnParams' => [
        'host' => '127.0.0.1:5757',
        'secret' => 'lorem.ipsum.dolor.sit.amet',
        'serial' => '98765432109876',
    ],
    'badSecretConnParams' => [
        'host' => '127.0.0.1:5757',
        'secret' => '!.BAD.SECRET.!',
        'serial' => '00106308750663',
    ],
    'badSerialConnParams' => [
        'host' => '127.0.0.1:5757',
        'secret' => 'lorem.ipsum.dolor.sit.amet',
        'serial' => '99106308755095',
    ],
    'goodConnString' => 'com,4',
    'badConnString1' => 'com,7',
    'badConnString2' => 'tcpip,172.0.0.1',
    'badConnString3' => 'abra,cad,abra',
    'operator' => [
        'name' => 'Оператрисса',
        // 'inn' => '770000000000',
    ],
    'customer' => [
        'account' => '123456789',
        'contact' => '+79602053380',
        'name' => 'Покупатель Товаров',
    ],
];
