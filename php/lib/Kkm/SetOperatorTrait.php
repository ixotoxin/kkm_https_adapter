<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

namespace Kkm;

trait SetOperatorTrait
{
    public function setOperator(string|array $name, ?string $inn = null): static
    {
        if (is_array($name)) {
            $inn = $name['inn'] ?? null;
            $name = $name['name'] ?? '';
        }
        $name = trim($name);
        if ($name) {
            $this->details['operator']['name'] = $name;
        } else {
            throw new \Exception();
        }
        if ($inn) {
            $this->details['operator']['inn'] = $inn;
        } else {
            unset($this->details['operator']['inn']);
        }
        return $this;
    }
}
