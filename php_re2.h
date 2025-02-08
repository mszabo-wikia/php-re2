/* re2 extension for PHP (c) 2025 Máté Szabó */

#ifndef PHP_RE2_H
#define PHP_RE2_H

#include <memory>
#include <re2/re2.h>

#include "zend_API.h"

extern zend_module_entry re2_module_entry;
#define phpext_re2_ptr &re2_module_entry

#define PHP_RE2_VERSION "0.1.0"

#if defined(ZTS) && defined(COMPILE_DL_RE2)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

typedef struct {
  std::shared_ptr<RE2> re2;
  zend_object std;
} re2_pattern_t;

#endif /* PHP_RE2_H */
