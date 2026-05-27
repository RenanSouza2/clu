#include "../debug.h"
#include "../../../testrc.h"
#include "../../../mods/macros/test.h"




void test_example()
{
    TEST_LIB

    bool show = false;

    TEST_ASSERT_MEM_EMPTY;
}


int main()
{
    setvbuf(stdout, nullptr, _IONBF, 0);
    test_example();
    fprintf(stderr, "\n\n\tTest successful\n\n");
    return 0;
}
