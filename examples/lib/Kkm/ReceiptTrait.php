<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

namespace Kkm;

defined('KKMHA_EXAMPLES') or die('No direct script access.');

trait ReceiptTrait
{
    public function setCustomer(
        string|array|null $accountOrArray,
        ?string $contact = null,
        ?string $name = null
    ): static {
        if (is_array($accountOrArray)) {
            $this->setCustomer(
                Utils::nullableString($accountOrArray['account'] ?? null),
                $accountOrArray['contact'] ?? null,
                $accountOrArray['name'] ?? null,
            );
            return $this;
        }
        if (isset($accountOrArray)) {
            $this->details['customer']['account'] = $accountOrArray;
        }
        if (isset($accountOrArray)) {
            $this->details['customer']['contact'] = $contact;
        }
        if (isset($accountOrArray)) {
            $this->details['customer']['name'] = $name;
        }
        return $this;
    }

    public function setTax(Tax|string $tax): static
    {
        if (is_string($tax)) {
            return $this->setTax(Tax::from($tax));
        }
        $this->details['tax'] = $tax->value;
        return $this;
    }

    public static function createItem(string $title, float $price, float $quantity, Tax|string|null $tax): array
    {
        $length = mb_strlen($title);
        if ($length < 1 || $length > 200) {
            throw new \RuntimeException('Название товара/услуги некорректно');
        }
        if ($price < 0.01 || $price > 999999.99) {
            throw new \RuntimeException('Цена некорректна');
        }
        if ($quantity < 0.001 || $quantity > 999999.999) {
            throw new \RuntimeException('Количество некорректно');
        }
        $item = [
            'title' => $title,
            'price' => $price,
            'quantity' => $quantity,
        ];
        $tax = Tax::cast($tax, true)?->value;
        if ($tax) {
            $item['tax'] = $tax;
        }
        return $item;
    }

    public function addItem(
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
    }

    public function setPayment(PaymentType|string|array $typeOrArray, ?float $sum = null): static
    {
        if (is_array($typeOrArray)) {
            $this->setPayment(PaymentType::cast($typeOrArray['type'] ?? null), $typeOrArray['sum'] ?? null);
            return $this;
        }
        $this->details['payment']['type'] = PaymentType::cast($typeOrArray)->value;
        $this->details['payment']['sum'] = $sum ?? 'auto';
        return $this;
    }

    public function addText(
        string $key,
        string|array|null $content,
        bool $center = false,
        bool $magnified = false,
        bool $separated = false,
    ): static {
        if (is_array($content)) {
            $center = $content['center'] ?? false;
            $magnified = $content['magnified'] ?? false;
            $separated = $content['separated'] ?? false;
            $content = Utils::nullableString($content['content'] ?? null);
        }
        $content = trim($content ?? '');
        if ($content == '') {
            unset($this->details[$key]);
        }
        $this->details[$key] = [
            'content' => $content,
            'center' => $magnified,
            'magnified' => $magnified,
            'separated' => $separated,
        ];
        return $this;
    }

    public function setText(
        string|array|null $content,
        bool $center = false,
        bool $magnified = false,
        bool $separated = false,
    ): static {
        return $this->addText('text', $content, $center, $magnified, $separated);
    }

    public function setHeaderText(
        string|array|null $content,
        bool $center = false,
        bool $magnified = false,
        bool $separated = false,
    ): static {
        return $this->addText('headerText', $content, $center, $magnified, $separated);
    }

    public function setFooterText(
        string|array|null $content,
        bool $center = false,
        bool $magnified = false,
        bool $separated = false,
    ): static {
        return $this->addText('footerText', $content, $center, $magnified, $separated);
    }
}
