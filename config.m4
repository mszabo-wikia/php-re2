PHP_ARG_WITH([re2],
  [for re2 support],
  [AS_HELP_STRING([--with-re2],
    [Include re2 support])])

AS_VAR_IF([PHP_RE2], [no],, [

  PHP_REQUIRE_CXX
  AC_LANG([C++])

  PHP_RE2_CXXFLAGS="-std=c++20 -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1 -Wall -Werror"

  PKG_CHECK_MODULES([RE2], [re2])

  PHP_EVAL_INCLINE([$RE2_CFLAGS])
  PHP_EVAL_LIBLINE([$RE2_LIBS], [RE2_SHARED_LIBADD])

  PHP_SUBST([RE2_SHARED_LIBADD])

  PHP_NEW_EXTENSION([re2],
    [re2.cpp LRUPatternCache.cpp],
    [$ext_shared],,
    [$PHP_RE2_CXXFLAGS])
])
