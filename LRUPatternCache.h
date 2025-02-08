#ifndef LRU_PATTERN_CACHE_H
#define LRU_PATTERN_CACHE_H

#include <list>
#include <memory>
#include <optional>
#include <re2/re2.h>
#include <string>
#include <string_view>
#include <unordered_map>

namespace RE2PHP {
// A simple cross-request LRU cache for RE2 patterns.
class LRUPatternCache {
 public:
  LRUPatternCache(std::size_t max_size) : max_size(max_size) {}

  std::optional<std::shared_ptr<RE2>> get(const std::string_view& pattern);

  void resize(std::size_t new_size);

  std::size_t size() const noexcept { return cache.size(); }

  void emplace(std::shared_ptr<RE2> re2);

 private:
  // Custom hasher to allow heterogenous lookups
  // using both std::string and std::string_view.
  struct StringHash {
    using is_transparent = void;

    std::size_t operator()(const std::string& str) const noexcept {
      return std::hash<std::string>{}(str);
    }

    std::size_t operator()(const std::string_view& str) const noexcept {
      return std::hash<std::string_view>{}(str);
    }
  };

  struct CacheEntry {
    std::shared_ptr<RE2> re2;
    // Iterator pointing to the key in the LRU access list.
    std::list<std::string>::iterator access_it;
  };

  std::unordered_map<std::string, CacheEntry, StringHash, std::equal_to<>>
      cache;
  std::list<std::string> access;
  std::size_t max_size;
};
}  // namespace RE2PHP

#endif
