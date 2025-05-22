<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

namespace Kkm;

enum SrvOp
{
    case Ping;
    case Learn;
    case ResetRegistry;
}
