find_path(RE2_INCLUDE_DIR re2/re2.h)
find_library(RE2_LIBRARY re2)

if (NOT RE2_INCLUDE_DIR OR NOT RE2_LIBRARY)
  message(FATAL_ERROR "RE2 library not found")
endif()
