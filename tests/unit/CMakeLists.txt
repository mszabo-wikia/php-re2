add_executable(
  php_re2_lru_pattern_cache_test
  LRUPatternCacheTest.cpp
)

target_link_libraries(
  php_re2_lru_pattern_cache_test
  PRIVATE
  php_re2_lru_pattern_cache
  GTest::gmock
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(php_re2_lru_pattern_cache_test)
