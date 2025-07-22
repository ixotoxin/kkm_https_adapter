<?php
// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

declare(strict_types=1);

namespace Kkm;

defined('KKMHA_EXAMPLES') or die('No direct script access.');

abstract class Performer
{
    const DEFAULT_JSON_ENCODE_OPTIONS =
        JSON_HEX_TAG | JSON_HEX_AMP | JSON_HEX_QUOT | JSON_HEX_APOS
        | JSON_INVALID_UTF8_IGNORE | JSON_NUMERIC_CHECK | JSON_PRESERVE_ZERO_FRACTION | JSON_THROW_ON_ERROR;

    public function __construct(
        public readonly ConnParams $connParams,
    ) {}

    public function performGetMethod(array|string $method, ?string $idempotencyKey): array
    {
        if (is_array($method)) {
            $method = implode('/', array_map(trim(...), $method));
        }
        $method = trim($method, '/\\');

        $idempotencyKey ??= Utils::uid();

        $curl = curl_init();
        curl_setopt($curl, CURLOPT_URL, 'https://' . $this->connParams->host . '/' . $method);
        curl_setopt($curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_setopt($curl, CURLOPT_SSL_VERIFYHOST, false);
        curl_setopt($curl, CURLOPT_HTTPHEADER, [
            'Content-Type: application/json',
            'X-Secret: ' . $this->connParams->secret,
            'X-Idempotency-Key: ' . $idempotencyKey
        ]);
        curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
        curl_setopt($curl, CURLOPT_HEADER, 1);

        $start = microtime(true);
        $response = curl_exec($curl);
        $stop = microtime(true);

        $error = curl_error($curl);
        if ($error) {
            curl_close($curl);
            return [
                '!message' => $error,
                '!success' => false,
                '!httpStatus' => 0,
                '!origin' => '',
                '!elapsedTime' => 0
            ];
        }

        $headerSize = curl_getinfo($curl, CURLINFO_HEADER_SIZE);
        $header = substr($response, 0, $headerSize);
        $origin = '';
        if (preg_match("/X-Origin: (\w+)/", $header, $matches)) {
            $origin = $matches[1];
        }
        $body = substr($response, $headerSize);
        $result = json_decode($body, true);

        if (!is_array($result)) {
            curl_close($curl);
            return [
                '!message' => 'Bad response',
                '!success' => false,
                '!httpStatus' => 0,
                '!origin' => $origin,
                '!elapsedTime' => 0
            ];
        }

        $result['!httpStatus'] = curl_getinfo($curl, CURLINFO_HTTP_CODE);
        $result['!origin'] = $origin;
        $result['!elapsedTime'] = ($stop - $start) * 1000;
        curl_close($curl);

        return $result;
    }

    public function performPostMethod(array|string $method, ?array $details, string $idempotencyKey): array
    {
        if (is_array($method)) {
            $method = implode('/', array_map(trim(...), $method));
        }
        $method = trim($method, '/\\');

        if (defined('OUT_REQUEST_BODY') && OUT_REQUEST_BODY) {
            echo '--- --- --- --- --- --- --- --- ---', PHP_EOL, '<pre>', PHP_EOL, 'requestBody = ', PHP_EOL;
            var_export($details);
            echo PHP_EOL, '--- --- --- --- --- --- --- --- ---', PHP_EOL, '<pre>';
        }
        $details = $details ? json_encode($details, self::DEFAULT_JSON_ENCODE_OPTIONS) : '{}';

        $curl = curl_init();
        curl_setopt($curl, CURLOPT_URL, 'https://' . $this->connParams->host . '/' . $method);
        curl_setopt($curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_setopt($curl, CURLOPT_SSL_VERIFYHOST, false);
        curl_setopt($curl, CURLOPT_HTTPHEADER, [
            'Content-Type: application/json',
            'X-Secret: ' . $this->connParams->secret,
            'X-Idempotency-Key: ' . $idempotencyKey
        ]);
        curl_setopt($curl, CURLOPT_POST, 1);
        curl_setopt($curl, CURLOPT_POSTFIELDS, $details);
        curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
        curl_setopt($curl, CURLOPT_HEADER, 1);

        $start = microtime(true);
        $response = curl_exec($curl);
        $stop = microtime(true);

        $error = curl_error($curl);
        if ($error) {
            curl_close($curl);
            return [
                '!message' => $error,
                '!success' => false,
                '!httpStatus' => 0,
                '!origin' => '',
                '!elapsedTime' => 0
            ];
        }

        $headerSize = curl_getinfo($curl, CURLINFO_HEADER_SIZE);
        $header = substr($response, 0, $headerSize);
        $origin = '';
        if (preg_match("/X-Origin: (\w+)/", $header, $matches)) {
            $origin = $matches[1];
        }
        $body = substr($response, $headerSize);
        $result = json_decode($body, true);

        if (!is_array($result)) {
            curl_close($curl);
            return [
                '!message' => 'Bad response',
                '!success' => false,
                '!httpStatus' => 0,
                '!origin' => $origin,
                '!elapsedTime' => 0
            ];
        }

        $result['!httpStatus'] = curl_getinfo($curl, CURLINFO_HTTP_CODE);
        $result['!origin'] = $origin;
        $result['!elapsedTime'] = ($stop - $start) * 1000;
        curl_close($curl);

        return $result;
    }
}
