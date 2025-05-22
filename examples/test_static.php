#!/usr/bin/php
<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

use Kkm\{ ConnParams, Device, DevOp, Utils };

require_once('lib.php');
$config = require('cfg.php');

function getStatic(ConnParams $connParams, string $path): void
{
    if ($path && $path !== '/') {
        $path = '/' . $path;
    }
    $url = 'https://' . $connParams->host . $path;
    echo "Try get URL '", $url, "'", PHP_EOL;

    $curl = curl_init();
    curl_setopt($curl, CURLOPT_URL, $url);
    curl_setopt($curl, CURLOPT_SSL_VERIFYPEER, false);
    curl_setopt($curl, CURLOPT_SSL_VERIFYHOST, false);
    curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);

    $start = microtime(true);
    $response = curl_exec($curl);
    $stop = microtime(true);

    $error = curl_error($curl);
    if ($error) {
        curl_close($curl);
        echo "Error: ", $error, PHP_EOL, PHP_EOL;
        return;
    }

    echo "Response code: ", curl_getinfo($curl, CURLINFO_HTTP_CODE), PHP_EOL, PHP_EOL;
    curl_close($curl);
    return ;
}

$tests = [
    '', '/', '//', '///', '.', '..', '...', '/.', '././.', 'abc..def', '../abc/',
    'index.html', 'static', '.static', 'static/.', 'static/.file', 'static/file.',
    'static/qwerty', 'static/abc..def', 'static/../abc/', 'static/index.html',
    'static/index.html', '//static///index.html', '././static/././index.html', '././static/././файл'
];

$connParams = new ConnParams($config['goodConnParams']);

echo PHP_EOL;
foreach ($tests as $path) {
    getStatic($connParams, $path);
}
