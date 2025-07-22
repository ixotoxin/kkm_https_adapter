<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

namespace Kkm;

defined('KKMHA_EXAMPLES') or die('No direct script access.');

enum DevOp
{
    case Status;
    case FullStatus;
    case PrintDemo;
    case PrintNonFiscalDoc;
    case PrintInfo;
    case PrintFnRegistrations;
    case PrintOfdExchangeStatus;
    case PrintOfdTest;
    case PrintCloseShiftReports;
    case PrintLastDocument;
    case CashStat;
    case CashIn;
    case CashOut;
    case CloseShift;
    case ResetState;
}
