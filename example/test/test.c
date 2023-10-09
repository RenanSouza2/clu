#include <stdio.h>
#include <assert.h>

#include "../debug.h"

void test_hello()
{
    printf("\n\t%s\t\t", __func__);

    hello_debug();
    hello_example();
}

void test_list_body()
{
    printf("\n%s\t\t", __func__);

    test_hello();
}


int main() 
{
    setbuf(stdout, NULL);
    test_list_body();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
