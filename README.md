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

public run_test2()
{
    UTEST_RUN(UT_VERBOSE)
}

public plugin_init()
{
    register_plugin("some_unit_test", "1.0", "alldroll")
    set_task(2.0, "run_test")
}

```
