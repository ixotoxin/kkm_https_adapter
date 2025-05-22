<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

namespace Kkm;

class Device extends Performer
{
    public function __construct(ConnParams $connParams)
    {
        if (!$connParams->serial) {
            throw new \Exception('Серийный номер ККМ некорректен');
        }
        parent::__construct($connParams);
    }

    public function prepare(DevOp $operation): \Kkm\DeviceOperation
    {
        return match ($operation) {
            DevOp::Status => new \Kkm\Operations\Status($this),
            DevOp::FullStatus => new \Kkm\Operations\FullStatus($this),
            DevOp::PrintDemo => new \Kkm\Operations\PrintDemo($this),
            DevOp::PrintNonFiscalDoc => new \Kkm\Operations\PrintNonFiscalDoc($this),
            DevOp::PrintInfo => new \Kkm\Operations\PrintInfo($this),
            DevOp::PrintFnRegistrations => new \Kkm\Operations\PrintFnRegistrations($this),
            DevOp::PrintOfdExchangeStatus => new \Kkm\Operations\PrintOfdExchangeStatus($this),
            DevOp::PrintOfdTest => new \Kkm\Operations\PrintOfdTest($this),
            DevOp::PrintCloseShiftReports => new \Kkm\Operations\PrintCloseShiftReports($this),
            DevOp::PrintLastDocument => new \Kkm\Operations\PrintLastDocument($this),
            DevOp::CashStat => new \Kkm\Operations\CashStat($this),
            DevOp::CashIn => new \Kkm\Operations\CashIn($this),
            DevOp::CashOut => new \Kkm\Operations\CashOut($this),
            DevOp::Sell => new \Kkm\Operations\Sell($this),
            DevOp::SellReturn => new \Kkm\Operations\SellReturn($this),
            DevOp::CloseShift => new \Kkm\Operations\CloseShift($this),
            DevOp::ReportX => new \Kkm\Operations\ReportX($this),
            DevOp::ResetState => new \Kkm\Operations\ResetState($this),
        };
    }

    public function status(): \Kkm\Operations\Status
    {
        return new \Kkm\Operations\Status($this);
    }

    public function fullStatus(): \Kkm\Operations\FullStatus
    {
        return new \Kkm\Operations\FullStatus($this);
    }

    public function printDemo(): \Kkm\Operations\PrintDemo
    {
        return new \Kkm\Operations\PrintDemo($this);
    }

    public function printNonFiscalDoc(): \Kkm\Operations\PrintNonFiscalDoc
    {
        return new \Kkm\Operations\PrintNonFiscalDoc($this);
    }

    public function printInfo(): \Kkm\Operations\PrintInfo
    {
        return new \Kkm\Operations\PrintInfo($this);
    }

    public function printFnRegistrations(): \Kkm\Operations\PrintFnRegistrations
    {
        return new \Kkm\Operations\PrintFnRegistrations($this);
    }

    public function printOfdExchangeStatus(): \Kkm\Operations\PrintOfdExchangeStatus
    {
        return new \Kkm\Operations\PrintOfdExchangeStatus($this);
    }

    public function printOfdTest(): \Kkm\Operations\PrintOfdTest
    {
        return new \Kkm\Operations\PrintOfdTest($this);
    }

    public function printCloseShiftReports(): \Kkm\Operations\PrintCloseShiftReports
    {
        return new \Kkm\Operations\PrintCloseShiftReports($this);
    }

    public function printLastDocument(): \Kkm\Operations\PrintLastDocument
    {
        return new \Kkm\Operations\PrintLastDocument($this);
    }

    public function cashStat(): \Kkm\Operations\CashStat
    {
        return new \Kkm\Operations\CashStat($this);
    }

    public function cashIn(): \Kkm\Operations\CashIn
    {
        return new \Kkm\Operations\CashIn($this);
    }

    public function cashOut(): \Kkm\Operations\CashOut
    {
        return new \Kkm\Operations\CashOut($this);
    }

    public function sell(): \Kkm\Operations\Sell
    {
        return new \Kkm\Operations\Sell($this);
    }

    public function sellReturn(): \Kkm\Operations\SellReturn
    {
        return new \Kkm\Operations\SellReturn($this);
    }

    public function closeShift(): \Kkm\Operations\CloseShift
    {
        return new \Kkm\Operations\CloseShift($this);
    }

    public function reportX(): \Kkm\Operations\ReportX
    {
        return new \Kkm\Operations\ReportX($this);
    }

    public function resetState(): \Kkm\Operations\ResetState
    {
        return new \Kkm\Operations\ResetState($this);
    }
}
