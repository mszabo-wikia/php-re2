cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(php_re2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH  "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

find_package(re2 REQUIRED)
find_package(GTest REQUIRED)

enable_testing()

add_library(
  php_re2_lru_pattern_cache
  LRUPatternCache.cpp
)
target_link_libraries(
  php_re2_lru_pattern_cache
  PUBLIC
  re2
)
target_include_directories(
  php_re2_lru_pattern_cache
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(tests/unit)
