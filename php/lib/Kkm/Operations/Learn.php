<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

namespace Kkm\Operations;

final class Learn extends \Kkm\ServerOperation implements \Kkm\PostOperation
{
    use \Kkm\PostOperationTrait;

    public const METHOD = 'kkm/learn';

    public function setConnParams(string $connParams): static
    {
        $this->details['connParams'] = $connParams;
        return $this;
    }
}
