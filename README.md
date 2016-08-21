Small pawm unit testing tools
===================

TODO add description


### How does it works:

```pawn

#include <amxmodx>
#include <utest>

stock max(a, b)
{
    return a > b ? a : b
}

stock min(a, b)
{
    return a > b ? b : a
}

TEST_LIST = {
    { "test1", "test max function" },
    { "test2", "test min function" },
    { "test3", "TODO" },
    TEST_LIST_END
};

START_TEST(test1) {
    ASSERT_INT_EQ(max(1, 3), 3)
    ASSERT_TRUE(max(1, 1) == 1)
    ASSERT_FALSE(max(-1, 1) == -1)
} END_TEST

START_TEST(test2) {
    ASSERT_INT_EQ(min(1, 3), 1)
    ASSERT_TRUE(min(1, 1) == 1)
    ASSERT_FALSE(min(-1, 1) == 1)
} END_TEST

START_TEST(test3) {
    SKIP_TEST("IMPLEMENT ME!")
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
>>> Running test suite [test5]: test max function
+ [test5] OK #1
+ [test5] OK #2
+ [test5] OK #3
>>> Running test suite [test6]: test min function
+ [test6] OK #1
+ [test6] OK #2
+ [test6] OK #3
>>> Running test suite [test7]: TODO
>>> Skip test suite test7 (IMPLEMENT ME!)
=======SUMMARY:========
Count of all unit tests: 3
Count of ok tests: 2
Count of failed tests: 0
Count of skipped tests: 1
SUCCESS: all unit tests have passed
==========END==========

```


