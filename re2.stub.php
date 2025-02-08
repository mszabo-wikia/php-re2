<?php

/** @generate-class-entries */

namespace RE2 {
    final class Pattern {
        public function __construct(string $pattern) {}

        public function matches(string $text): bool {}

        public function captures(string $text): array {}

        public static function quote(string $text): string {}
    }
}
