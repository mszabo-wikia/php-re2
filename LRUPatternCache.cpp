#include "LRUPatternCache.h"
namespace RE2PHP {
std::optional<std::shared_ptr<RE2>> LRUPatternCache::get(
    const std::string_view& pattern) {
  auto it = cache.find(pattern);
  if (it == cache.end()) {
    return std::nullopt;
  }

  auto cache_entry = it->second;

  access.erase(cache_entry.access_it);
  access.push_front(cache_entry.re2->pattern());
  cache_entry.access_it = access.begin();

  return cache_entry.re2;
}

void LRUPatternCache::emplace(std::shared_ptr<RE2> re2) {
  if (cache.size() >= max_size) {
    auto last = access.back();
    cache.erase(last);
    access.pop_back();
  }

  access.push_front(re2->pattern());
  cache.emplace(re2->pattern(), CacheEntry{re2, access.begin()});
}
}  // namespace RE2PHP
