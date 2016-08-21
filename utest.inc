#if defined _utest_included
    #endinput
#endif
#define _utest_included

#define TEST_NAME_MAX_LEN 64
#define TEST_FUNC_MAX_LEN 66

/* TODO */
#define UT_STOP_AFTER_FIRST_FAIL (1 << 0)
#define UT_VERBOSE (1 << 1)
#define UT_DEFAULT (UT_VERBOSE | UT_STOP_AFTER_FIRST_FAIL)

/**
 *
 */
enum _:_ScopeT {
    name[TEST_FUNC_MAX_LEN],
    total,
    options,
    skipped,
    ok,
    failed
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
    name[TEST_NAME_MAX_LEN],
    desc[200],
    func[TEST_FUNC_MAX_LEN],
    assert_number,
    TestResultT:result
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
    server_print(">>> Running test suite [%s]: %s", #%1, __test[desc]);

/**
 * 
 */
#define END_TEST }

/**
 * @param flags
 */
#define UTEST_RUN(%1) utest_run(g_utlist, %1)

/**
 * @param reason
 */
#define SKIP_TEST(%1)\
    server_print(">>> Skip test suite %s (%s)", __test[name], %1);\
    __test[result] = TEST_SKIP;\
    return

/**
 * @param ut_list[][_TestT]
 * @param flags
 */
stock utest_run(ut_list[][_TestT], flags = UT_DEFAULT)
{
    new TestResultT:res = TEST_UNDEFINED

    new __scope[_ScopeT] = {"", 0}
    new stop = false

    for (new i = 0; ut_list[i][name] != 0 && !stop; ++i) {
        ++__scope[total]
        ut_list[i][result] = TEST_OK

        format(ut_list[i][func], TEST_FUNC_MAX_LEN, "__%s", ut_list[i][name])
        if (!callfunc_begin(ut_list[i][func])) {
            /* TODO log error*/
        }

        callfunc_push_array(_:ut_list[i], _TestT)
        callfunc_end()

        res = ut_list[i][result]
        switch (res) {
            case TEST_OK: ++__scope[ok];
            case TEST_FAIL: {
                ++__scope[failed];
                stop = flags & UT_STOP_AFTER_FIRST_FAIL
            }
            case TEST_SKIP: ++__scope[skipped];
        }
    }

    if (flags & UT_VERBOSE) {
        server_print("=======Summary:========")
        server_print("Count of all unit tests: %d", __scope[total])
        server_print("Count of ok tests: %d", __scope[ok])
        server_print("Count of failed tests: %d", __scope[failed])
        server_print("Count of skipped tests: %d", __scope[skipped])
    }

    if (__scope[failed]) {
        server_print(
            "FAILED: %d of %d unit tests have failed",
            __scope[failed],
            __scope[total]
        )
    } else {
        server_print("SUCCESS: all unit tests have passed")
    }
}

#define ASSERT_TRUE(%1) test_assert(__test, any:%1, __LINE__)

#define ASSERT_TRUE_MSG(%1,%2) test_assert(__test, any:%1, __LINE__, %2)

#define ASSERT_FALSE(%1) ASSERT_TRUE(!%1)

#define ASSERT_FALSE_MSG(%1,%2) ASSERT_TRUE(!%1,%2)

#define ASSERT_INT(%1,%2,%3) ASSERT_TRUE(cell:%1%2cell:%3)

#define ASSERT_INT_EQ(%1,%2) ASSERT_INT(%1,==,%2)

#define ASSERT_INT_NEQ(%1,%2) ASSERT_INT(%1,!=,%2)

#define ASSERT_INT_G(%1,%2) ASSERT_INT(%1,>,%2)

#define ASSERT_INT_GE(%1,%2) ASSERT_INT(%1,>=,%2)

#define ASSERT_INT_L(%1,%2) ASSERT_INT(%1,<,%2)

#define ASSERT_INT_LE(%1,%2) ASSERT_INT(%1,<=,%2)

stock test_assert(test[_TestT], cond, line, const message[] = "")
{
    new number = ++test[assert_number]

    if (cond) {
        server_print("[%s] OK #%d", test[name], number)
    } else {
        new msg[256] = ""
        if (strlen(message)) {
            formatex(msg, sizeof(msg), ", %s", message)
        }

        server_print(
            "[%s] FAIL #%d LINE %d%s",
            test[name],
            number,
            line,
            msg
        )
        test[result] = TEST_FAIL
    }
}

#undef TEST_FUNC_MAX_LEN
#undef TEST_NAME_MAX_LEN
