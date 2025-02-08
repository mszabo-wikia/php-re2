--TEST--
Resizing pattern cache at runtime
--EXTENSIONS--
re2
--INI--
re2.max_pattern_cache_size=8
--FILE--
<?php
$patterns = [];
for ($i = 1; $i <= 8; $i++) {
    $patterns[] = new RE2\Pattern('pattern' . $i);
}

ini_set('re2.max_pattern_cache_size', '4');

var_dump($patterns[0]->matches('pattern1'));
--EXPECT--
bool(true)
