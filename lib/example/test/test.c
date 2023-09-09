#include <stdio.h>
#include <assert.h>

#include "../debug.h"

void test_hello()
{
    printf("\n\ttest hello\t\t");

    hello_debug();
    hello_example();
}

void test_example()
{
    printf("\ntest example library\t\t");

    test_hello();
}


int main() 
{
    setbuf(stdout, NULL);
    test_example();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
