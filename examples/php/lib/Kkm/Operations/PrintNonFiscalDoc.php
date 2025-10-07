<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

namespace Kkm\Operations;

defined('KKMHA_EXAMPLES') or die('No direct script access.');

// TODO: Реализовать.
final class PrintNonFiscalDoc extends \Kkm\DeviceOperation implements \Kkm\PostOperation
{
    use \Kkm\PostOperationTrait;

    public const METHOD = 'print-non-fiscal-doc';

    /* public function addItem(
        string|array $title,
        int|float $price = 0,
        int|float $quantity = 1,
        Tax|string|null $tax = null
    ): static {
        if (is_array($title)) {
            $price = $title['price'] ?? 0;
            $quantity = $title['quantity'] ?? 1;
            $tax = $title['tax'] ?? null;
            $title = Utils::nullableString($title['title'] ?? null);
        }
        $this->details['items'][] = static::createItem($title, $price, $quantity, $tax);
        return $this;
    }

    public function setItems(array $items): static
    {
        foreach ($items as $item) {
            $this->addItem($item);
        }
        return $this;
    } */
}
