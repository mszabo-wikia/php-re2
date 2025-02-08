--TEST--
Quoting metacharacters
--EXTENSIONS--
re2
--FILE--
<?php
var_dump(RE2\Pattern::quote('a.b'));
var_dump(RE2\Pattern::quote(''));
?>
--EXPECT--
string(4) "a\.b"
string(0) ""
