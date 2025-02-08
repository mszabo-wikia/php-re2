--TEST--
Basic match
--EXTENSIONS--
re2
--FILE--
<?php
$pattern = new RE2\Pattern('Hello');
var_dump($pattern->matches('Hello World'));
?>
--EXPECT--
bool(true)
