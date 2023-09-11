#include <stdio.h>
#include <assert.h>

#include "../debug.h"

void test_hello()
{
    printf("\n\ttest hello\t\t");
}

void test_root()
{
    printf("\ntest root library\t\t");

    test_hello();
}


int main() 
{
    setbuf(stdout, NULL);
    test_root();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
