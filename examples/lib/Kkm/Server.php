<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

namespace Kkm;

class Server extends Performer
{
    public function __construct(ConnParams $connParams)
    {
        parent::__construct($connParams);
    }

    public function prepare(SrvOp $operation): \Kkm\ServerOperation
    {
        return match ($operation) {
            SrvOp::Learn => new \Kkm\Operations\Learn($this),
            SrvOp::ResetRegistry => new \Kkm\Operations\ResetRegistry($this),
            SrvOp::Ping => new \Kkm\Operations\Ping($this),
        };
    }

    public function learn(): \Kkm\Operations\Learn
    {
        return new \Kkm\Operations\Learn($this);
    }

    public function resetRegistry(): \Kkm\Operations\ResetRegistry
    {
        return new \Kkm\Operations\ResetRegistry($this);
    }

    public function ping(): \Kkm\Operations\Ping
    {
        return new \Kkm\Operations\Ping($this);
    }
}
