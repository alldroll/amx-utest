Primitive pawn unit testing tools
===================

TODO add description


### How does it works:

```pawn

#include <amxmodx>
#include <utest>

stock my_max(a, b)
{
    return a > b ? a : b
}

stock my_min(a, b)
{
    return a > b ? b : a
}

TEST_LIST = {
    { "test1", "test max function" },
    { "test2", "test min function" },
    { "test3", "skipped test" },
    { "test4", "bad test" },
    TEST_LIST_END
};

START_TEST(test1) {
    ASSERT_INT_EQ(my_max(1, 3), 3)
    ASSERT_TRUE(my_max(1, 1) == 1)
    ASSERT_FALSE(my_max(-1, 1) == -1)
} END_TEST

START_TEST(test2) {
    ASSERT_INT_EQ_MSG(my_min(1, 3), 1, "my_min(1, 3) must return 1")
    ASSERT_TRUE(my_min(1, 1) == 1)
    ASSERT_FALSE(my_min(-1, 1) == 1)
} END_TEST

START_TEST(test3) {
    SKIP_TEST("IMPLEMENT ME!")
} END_TEST

START_TEST(test4) {
    ASSERT_TRUE_MSG(false, "show how test fail looks") /* line 38 */
} END_TEST

public run_test()
{
    UTEST_RUN(UT_VERBOSE)
}

public plugin_init()
{
    register_plugin("some_unit_test", "1.0", "alldroll")
    set_task(2.0, "run_test")
}

OUTPUT:

======UTEST RUN=======
>>> Running test suite [test1]: test max function
+ [test1] OK #1
+ [test1] OK #2
+ [test1] OK #3
>>> Running test suite [test2]: test min function
+ [test2] OK #1
+ [test2] OK #2
+ [test2] OK #3
>>> Running test suite [test3]: skipped test
>>> Skip test suite test3 (IMPLEMENT ME!)
>>> Running test suite [test4]: bad test
+ [test4] FAIL #1 LINE 38, show how test fail looks
=======SUMMARY:========
Count of all unit tests: 4
Count of ok tests: 2
Count of failed tests: 1
Count of skipped tests: 1
FAILED: 1 of 4 unit tests have failed
==========END==========

```


