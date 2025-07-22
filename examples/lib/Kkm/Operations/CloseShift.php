<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

namespace Kkm\Operations;

defined('KKMHA_EXAMPLES') or die('No direct script access.');

final class CloseShift extends \Kkm\DeviceOperation implements \Kkm\PostOperation
{
    use \Kkm\PostOperationTrait;
    use \Kkm\SetOperatorTrait;

    public const METHOD = 'close-shift';

    public function setCashOut(bool $cashOut): static
    {
        $this->details['cashOut'] = $cashOut;
        return $this;
    }
}
