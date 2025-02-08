--TEST--
Invalid pattern creation
--EXTENSIONS--
re2
--FILE--
<?php
try {
    $pattern = new RE2\Pattern('(.)\1');
} catch (ValueError $e) {
    echo $e->getMessage(), PHP_EOL;
}
?>
--EXPECT--
invalid escape sequence: \1
