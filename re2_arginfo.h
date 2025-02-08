/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 93c89a67fd8a175ac6afa46c7ed3c654d1687f3d */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_RE2_Pattern___construct, 0, 0, 1)
ZEND_ARG_TYPE_INFO(0, pattern, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_RE2_Pattern_matches, 0, 1,
                                        _IS_BOOL, 0)
ZEND_ARG_TYPE_INFO(0, text, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_RE2_Pattern_captures, 0,
                                        1, IS_ARRAY, 0)
ZEND_ARG_TYPE_INFO(0, text, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_METHOD(RE2_Pattern, __construct);
ZEND_METHOD(RE2_Pattern, matches);
ZEND_METHOD(RE2_Pattern, captures);

static const zend_function_entry class_RE2_Pattern_methods[] = {
    ZEND_ME(RE2_Pattern, __construct, arginfo_class_RE2_Pattern___construct,
            ZEND_ACC_PUBLIC)
        ZEND_ME(RE2_Pattern, matches, arginfo_class_RE2_Pattern_matches,
                ZEND_ACC_PUBLIC)
            ZEND_ME(RE2_Pattern, captures, arginfo_class_RE2_Pattern_captures,
                    ZEND_ACC_PUBLIC) ZEND_FE_END};

static zend_class_entry *register_class_RE2_Pattern(void) {
  zend_class_entry ce, *class_entry;

  INIT_NS_CLASS_ENTRY(ce, "RE2", "Pattern", class_RE2_Pattern_methods);
  class_entry = zend_register_internal_class_ex(&ce, NULL);
  class_entry->ce_flags |= ZEND_ACC_FINAL;

  return class_entry;
}
