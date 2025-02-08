--TEST--
Basic captures extraction
--EXTENSIONS--
re2
--FILE--
<?php
$pattern = new RE2\Pattern('(foo)|(bar)baz');
var_dump($pattern->captures('barbazbla'));
?>
--EXPECT--
array(3) {
  [0]=>
  string(6) "barbaz"
  [1]=>
  NULL
  [2]=>
  string(3) "bar"
}
