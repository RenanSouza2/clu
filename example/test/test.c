#include <stdio.h>

#include "../debug.h"

void test_hello()
{
    printf("\n\t%s", __func__);

    hello_debug();
    hello_example();
}

void test_list_body()
{
    printf("\n%s", __func__);

    test_hello();
}


int main()
{
    setbuf(stdout, NULL);
    test_list_body();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
