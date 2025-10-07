<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

spl_autoload_register(
    fn ($class) => include_once(realpath(__DIR__ . DIRECTORY_SEPARATOR . 'lib' . DIRECTORY_SEPARATOR . $class . '.php'))
);

defined('KKMHA_EXAMPLES') or define('KKMHA_EXAMPLES', true);
