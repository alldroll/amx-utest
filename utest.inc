#if defined _utest_included
    #endinput
#endif
#define _utest_included

/**
 * inspired by https://github.com/mity/cutest
 * inspired by https://github.com/libcheck/check
 */

#define TEST_NAME_MAX_LEN 64
#define TEST_FUNC_MAX_LEN 66
#define TEST_DESCRIPTION_MAX_LEN 255

/**
 */
#define UT_STOP_AFTER_FIRST_FAIL (1 << 0)
#define UT_VERBOSE (1 << 1)
#define UT_DEFAULT (UT_VERBOSE | UT_STOP_AFTER_FIRST_FAIL)

/**
 *
 */
enum _:_ScopeT {
    _es_name[TEST_FUNC_MAX_LEN],
    _es_total,
    _es_flags,
    _es_skipped,
    _es_passed,
    _es_failed
}

/**
 *
 */
enum TestResultT {
    TEST_OK = 0,
    TEST_FAIL,
    TEST_SKIP,
    TEST_UNDEFINED
};

/**
 *
 */
enum _:_TestT {
    _et_name[TEST_NAME_MAX_LEN],
    _et_desc[TEST_DESCRIPTION_MAX_LEN],
    _et_func[TEST_FUNC_MAX_LEN],
    _et_assert_number,
    TestResultT:_et_result
};

/**
 *
 */
#define TEST_LIST_END { 0 }

/**
 *
 */
#define TEST_LIST stock g_utlist[][_TestT]


/**
 *
 */
#define START_TEST(%1)\
public __%1(__test[_TestT]) {\
    server_print(">>> Running test suite [%s]: %s", #%1, __test[_et_desc]);

/**
 *
 */
#define END_TEST }

/**
 * @param flags
 */
#define UTEST_RUN(%1) utest_run(g_utlist, %1)

/**
 * @param reason */
#define SKIP_TEST(%1)\
    server_print(">>> Skip test suite %s (%s)", __test[_et_name], %1);\
    __test[_et_result] = TEST_SKIP;\
    return

/**
 * @param ut_list[][_TestT]
 * @param flags
 */
stock utest_run(ut_list[][_TestT], flags = UT_DEFAULT)
{
    new TestResultT:res = TEST_UNDEFINED

    new scope[_ScopeT] = {"", 0}
    new stop = false

    server_print("======UTEST RUN=======")

    for (new i = 0; ut_list[i][_et_name] != 0 && !stop; ++i) {
        ++scope[_es_total]
        ut_list[i][_et_result] = TEST_OK

        format(ut_list[i][_et_func], TEST_FUNC_MAX_LEN, "__%s", ut_list[i][_et_name])
        if (!callfunc_begin(ut_list[i][_et_func])) {
            /* TODO log error*/
        }

        callfunc_push_array(_:ut_list[i], _TestT)
        callfunc_end()

        res = ut_list[i][_et_result]
        switch (res) {
            case TEST_OK: ++scope[_es_passed];
            case TEST_FAIL: {
                ++scope[_es_failed];
                stop = flags & UT_STOP_AFTER_FIRST_FAIL
            }
            case TEST_SKIP: ++scope[_es_skipped];
        }
    }

    if (flags & UT_VERBOSE) {
        server_print("=======SUMMARY:========")
        server_print("Count of all unit tests: %d", scope[_es_total])
        server_print("Count of ok tests: %d", scope[_es_passed])
        server_print("Count of failed tests: %d", scope[_es_failed])
        server_print("Count of skipped tests: %d", scope[_es_skipped])
    }

    if (scope[_es_failed]) {
        server_print(
            "FAILED: %d of %d unit tests have failed",
            scope[_es_failed],
            scope[_es_total]
        )
    } else {
        server_print("SUCCESS: all unit tests have passed")
    }

    server_print("==========END==========")
}

/**
 *
 */
#define ASSERT_TRUE(%1) _test_assert(__test, any:%1, __LINE__)

/**
 *
 */
#define ASSERT_TRUE_MSG(%1,%2) _test_assert(__test, any:%1, __LINE__, %2)

/**
 *
 */
#define ASSERT_FALSE(%1) ASSERT_TRUE(%1 == false)

/**
 *
 */
#define ASSERT_FALSE_MSG(%1,%2) ASSERT_TRUE(%1 == false,%2)

/**
 *
 */
#define ASSERT_INT(%1,%2,%3) ASSERT_TRUE(cell:%1%2cell:%3)

/**
 *
 */
#define ASSERT_INT_EQ(%1,%2) ASSERT_INT(%1,==,%2)

/**
 *
 */
#define ASSERT_INT_NEQ(%1,%2) ASSERT_INT(%1,!=,%2)

/**
 *
 */
#define ASSERT_INT_G(%1,%2) ASSERT_INT(%1,>,%2)

/**
 *
 */
#define ASSERT_INT_GE(%1,%2) ASSERT_INT(%1,>=,%2)

/**
 *
 */
#define ASSERT_INT_L(%1,%2) ASSERT_INT(%1,<,%2)

/**
 *
 */
#define ASSERT_INT_LE(%1,%2) ASSERT_INT(%1,<=,%2)

/**
 *
 */
stock _test_assert(test[_TestT], cond, line, const message[] = "")
{
    new number = ++test[_et_assert_number]

    if (cond) {
        server_print("+ [%s] OK #%d", test[_et_name], number)
    } else {
        new msg[256] = ""
        if (strlen(message)) {
            formatex(msg, sizeof(msg), ", %s", message)
        }

        server_print(
            "- [%s] FAIL #%d LINE %d%s",
            test[_et_name],
            number,
            line,
            msg
        )
        test[_et_result] = TEST_FAIL
    }
}

#undef TEST_FUNC_MAX_LEN
#undef TEST_NAME_MAX_LEN
#undef TEST_DESCRIPTION_MAX_LEN
