#include <stdio.h>
#include <stdbool.h>

#include "../debug.h"

#include "../../../../mods/macros/test.h"



#define TEST_ASSERT_EMPTY assert(clu_mem_internal_empty());

void test_example()
{
    TEST_LIB

    bool show = false;

    TEST_ASSERT_EMPTY;
}


int main()
{
    setbuf(stdout, NULL);
    TEST_TIMEOUT_OPEN(TEST_TIMEOUT_DEFAULT)
    test_example();
    TEST_TIMEOUT_CLOSE
    printf("\n\n\tTest successful\n\n");
    return 0;
}
