<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

namespace Kkm;

trait PostOperationTrait
{
    use SetIdempotencyKeyTrait;

    protected array $details = [];

    public function setPadding(string $padding): static
    {
        $this->details['*padding*'] = $padding;
        return $this;
    }

    public function setDetails(array $details, bool $ignoreUnknown = false): static
    {
        foreach ($details as $key => $value) {
            if (preg_match("/^details$/i", $key)) {
                continue;
            }
            $method = 'set' . $key;
            if (method_exists($this, $method)) {
                $this->{$method}($value);
            } elseif (!$ignoreUnknown) {
                throw new \Exception('Неизвестный ключ');
            }
        }
        return $this;
    }

    public function perform(array $details = []): array
    {
        if ($details) {
            $this->setDetails($details);
        }
        if (!$this->idempotencyKey) {
            throw new \Exception('Ключ идемпотентности некорректен');
        }
        return $this->performer->performPostMethod($this->method(), $this->details, $this->idempotencyKey);
    }
}
