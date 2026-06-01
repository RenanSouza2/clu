#include "../../../testrc.h"
#include "../../../mods/macros/test.h"



static void test_mem_fn(bool show)
{
    TEST_FN_OPEN

    TEST_CASE_OPEN(1)
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}



static void test_list()
{
    TEST_LIB

    bool show = false;

    test_mem_fn(show);

    TEST_ASSERT_MEM_EMPTY
}



int main()
{
    setvbuf(stdout, nullptr, _IONBF, 0);
    test_list();
    fprintf(stderr, "\n\n\tTest successful\n\n");
    return 0;
}
