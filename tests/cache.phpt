--TEST--
Caching
--EXTENSIONS--
re2
--FILE--
<?php
$patterns = [];
for ($i = 1; $i <= 260; $i++) {
    if ( $i === 100) {
        $patterns[] = new RE2\Pattern('Hello2');
    } else {
        $patterns[] = new RE2\Pattern('Hello' . $i);
    }
}

foreach ($patterns as $pattern) {
    $pattern->matches('Hello World');
}

?>
--EXPECT--
