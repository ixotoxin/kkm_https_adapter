<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

spl_autoload_register(
    function ($class)
    {
        $fileName = realpath(__DIR__ . DIRECTORY_SEPARATOR . 'lib' . DIRECTORY_SEPARATOR . $class . '.php');
        $fileName and require_once($fileName);
        return $fileName;
    }
);
