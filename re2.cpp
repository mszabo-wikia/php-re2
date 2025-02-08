#include "php.h"

#include <memory>
#include <re2/stringpiece.h>
#include <string_view>
#include <vector>

#include "LRUPatternCache.h"
#include "Zend/zend_exceptions.h"
#include "zend_API.h"
#include "zend_hash.h"
#include "zend_portability.h"
#include "zend_types.h"
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <re2/re2.h>

#include "ext/standard/info.h"
#include "php_re2.h"
#include "re2_arginfo.h"

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
  ZEND_PARSE_PARAMETERS_START(0, 0)  \
  ZEND_PARSE_PARAMETERS_END()
#endif

static zend_class_entry* re2_pattern_ce;
static zend_object_handlers re2_pattern_handlers;

static zend_always_inline re2_pattern_t* re2_pattern_from_obj(
    zend_object* obj) {
  return reinterpret_cast<re2_pattern_t*>(reinterpret_cast<char*>(obj) -
                                          XtOffsetOf(re2_pattern_t, std));
}

static zend_always_inline re2_pattern_t* re2_pattern_from_zval(zval* zv) {
  return re2_pattern_from_obj(Z_OBJ_P(zv));
}

static zend_object* re2_pattern_create(zend_class_entry* ce) {
  re2_pattern_t* intern = reinterpret_cast<re2_pattern_t*>(
      zend_object_alloc(sizeof(re2_pattern_t), ce));

  zend_object_std_init(&intern->std, ce);
  object_properties_init(&intern->std, ce);

  intern->std.handlers = &re2_pattern_handlers;

  return &intern->std;
}

static void re2_pattern_free(zend_object* obj) {
  re2_pattern_t* intern = re2_pattern_from_obj(obj);

  // Reduce the refcount of the pattern held by this object.
  std::shared_ptr<RE2> re2 = std::move(intern->re2);
}

ZEND_BEGIN_MODULE_GLOBALS(re2)
std::unique_ptr<RE2PHP::LRUPatternCache> pattern_cache;
ZEND_END_MODULE_GLOBALS(re2)

ZEND_DECLARE_MODULE_GLOBALS(re2)

#define RE2_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(re2, v)

PHP_METHOD(RE2_Pattern, __construct) {
  zend_string* pattern;

  ZEND_PARSE_PARAMETERS_START(1, 1)
  Z_PARAM_STR(pattern)
  ZEND_PARSE_PARAMETERS_END();

  const std::string_view pattern_sv{ZSTR_VAL(pattern), ZSTR_LEN(pattern)};

  re2_pattern_t* intern = re2_pattern_from_zval(ZEND_THIS);

  // Try to reuse an existing cached pattern if possible
  if (auto cached_pattern = RE2_G(pattern_cache)->get(pattern_sv)) {
    intern->re2 = *cached_pattern;
    return;
  }

  std::shared_ptr<RE2> re2 = std::make_shared<RE2>(pattern_sv, RE2::Quiet);

  if (!re2->ok()) {
    zend_throw_exception_ex(zend_ce_value_error, 0, "%s", re2->error().c_str());
    RETURN_THROWS();
  }

  RE2_G(pattern_cache)->emplace(re2);

  intern->re2 = re2;
}

PHP_METHOD(RE2_Pattern, matches) {
  zend_string* subject;

  ZEND_PARSE_PARAMETERS_START(1, 1)
  Z_PARAM_STR(subject)
  ZEND_PARSE_PARAMETERS_END();

  re2_pattern_t* intern = re2_pattern_from_zval(ZEND_THIS);

  RETURN_BOOL(RE2::PartialMatch(ZSTR_VAL(subject), *intern->re2));
}

PHP_METHOD(RE2_Pattern, captures) {
  zend_string* subject;

  ZEND_PARSE_PARAMETERS_START(1, 1)
  Z_PARAM_STR(subject)
  ZEND_PARSE_PARAMETERS_END();

  re2_pattern_t* intern = re2_pattern_from_zval(ZEND_THIS);

  std::size_t capture_count = intern->re2->NumberOfCapturingGroups() + 1;

  std::vector<re2::StringPiece> submatches{capture_count};

  bool matched = intern->re2->Match(ZSTR_VAL(subject), 0, ZSTR_LEN(subject),
                                    RE2::Anchor::UNANCHORED, submatches.data(),
                                    capture_count);

  if (!matched) {
    RETURN_EMPTY_ARRAY();
  }

  HashTable* captures = zend_new_array(capture_count);

  for (const auto& submatch : submatches) {
    zval zv;

    if (submatch.empty()) {
      ZVAL_NULL(&zv);
    } else {
      ZVAL_STRINGL(&zv, submatch.data(), submatch.size());
    }

    zend_hash_next_index_insert(captures, &zv);
  }

  RETURN_ARR(captures);
}

PHP_METHOD(RE2_Pattern, quote) {
  zend_string* text;

  ZEND_PARSE_PARAMETERS_START(1, 1)
  Z_PARAM_STR(text)
  ZEND_PARSE_PARAMETERS_END();

  std::string_view text_sv{ZSTR_VAL(text), ZSTR_LEN(text)};

  if (text_sv.empty()) {
    RETURN_EMPTY_STRING();
  }

  std::string quoted = RE2::QuoteMeta(text_sv);

  RETURN_STRINGL(quoted.c_str(), quoted.size());
}

PHP_RINIT_FUNCTION(re2) {
#if defined(ZTS) && defined(COMPILE_DL_RE2)
  ZEND_TSRMLS_CACHE_UPDATE();
#endif

  return SUCCESS;
}

PHP_MINIT_FUNCTION(re2) {
  re2_pattern_ce = register_class_RE2_Pattern();
  re2_pattern_ce->create_object = re2_pattern_create;

  memcpy(&re2_pattern_handlers, &std_object_handlers,
         sizeof(zend_object_handlers));
  re2_pattern_handlers.offset = XtOffsetOf(re2_pattern_t, std);
  re2_pattern_handlers.free_obj = re2_pattern_free;

  RE2_G(pattern_cache) = std::make_unique<RE2PHP::LRUPatternCache>(256);

  return SUCCESS;
}

PHP_MINFO_FUNCTION(re2) {
  php_info_print_table_start();
  php_info_print_table_row(2, "re2 support", "enabled");
  php_info_print_table_end();
}

zend_module_entry re2_module_entry = {
    STANDARD_MODULE_HEADER,
    "re2",           /* Extension name */
    NULL,            /* zend_function_entry */
    PHP_MINIT(re2),  /* PHP_MINIT - Module initialization */
    NULL,            /* PHP_MSHUTDOWN - Module shutdown */
    PHP_RINIT(re2),  /* PHP_RINIT - Request initialization */
    NULL,            /* PHP_RSHUTDOWN - Request shutdown */
    PHP_MINFO(re2),  /* PHP_MINFO - Module info */
    PHP_RE2_VERSION, /* Version */
    STANDARD_MODULE_PROPERTIES};

#ifdef COMPILE_DL_RE2
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(re2)
#endif
