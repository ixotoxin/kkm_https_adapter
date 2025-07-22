<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

defined('KKMHA_EXAMPLES') or die('Include first lib.php.');

return require(getenv('KKMHA_EXAMPLES_CONFIG') ?: (__DIR__ . DIRECTORY_SEPARATOR . '/cfg_public.php'));
