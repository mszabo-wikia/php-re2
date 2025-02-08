#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "LRUPatternCache.h"

namespace RE2PHP {
TEST(LRUPatternCacheTest, basicRetrieval) {
  LRUPatternCache cache{2};

  auto re2 = std::make_shared<RE2>("a");
  cache.emplace(re2);
  auto retrieved = cache.get("a");

  EXPECT_THAT(retrieved, testing::Optional(re2));
}

TEST(LRUPatternCacheTest, evictLeastRecentlyUsedOnFull) {
  LRUPatternCache cache{2};

  auto first = std::make_shared<RE2>("a");
  auto second = std::make_shared<RE2>("b");

  cache.emplace(first);
  cache.emplace(second);

  // Access the first pattern, moving it to the front of the LRU list.
  EXPECT_THAT(cache.get("a"), testing::Optional(first));

  auto third = std::make_shared<RE2>("c");
  cache.emplace(third);

  EXPECT_THAT(cache.get("a"), testing::Optional(first));
  EXPECT_THAT(cache.get("b"), testing::Eq(std::nullopt));
  EXPECT_THAT(cache.get("c"), testing::Optional(third));
}

TEST(LRUPatternCacheTest, evictLeastRecentlyUsedOnDownsize) {
  LRUPatternCache cache{3};

  auto first = std::make_shared<RE2>("a");
  auto second = std::make_shared<RE2>("b");
  auto third = std::make_shared<RE2>("c");

  cache.emplace(first);
  cache.emplace(second);
  cache.emplace(third);

  EXPECT_THAT(cache.get("a"), testing::Optional(first));
  EXPECT_THAT(cache.get("b"), testing::Optional(second));
  EXPECT_THAT(cache.get("c"), testing::Optional(third));

  cache.resize(2);

  EXPECT_THAT(cache.get("a"), testing::Eq(std::nullopt));
  EXPECT_THAT(cache.get("b"), testing::Optional(second));
  EXPECT_THAT(cache.get("c"), testing::Optional(third));
}

TEST(LRUPatternCacheTest, preserveCacheOnUpsize) {
  LRUPatternCache cache{2};

  auto first = std::make_shared<RE2>("a");
  auto second = std::make_shared<RE2>("b");

  cache.emplace(first);
  cache.emplace(second);

  cache.resize(3);

  EXPECT_THAT(cache.get("a"), testing::Optional(first));
  EXPECT_THAT(cache.get("b"), testing::Optional(second));
}

TEST(LRUPatternCacheTest, canDownsizeEmptyCache) {
  LRUPatternCache cache{2};

  cache.resize(1);

  EXPECT_THAT(cache.size(), testing::Eq(0));
}

TEST(LRUPatternCacheTest, canDownsizeEmptyCacheToZeroSize) {
  LRUPatternCache cache{5};

  cache.resize(0);

  EXPECT_THAT(cache.size(), testing::Eq(0));
}
}  // namespace RE2PHP
