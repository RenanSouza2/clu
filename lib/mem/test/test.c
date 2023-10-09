#include <stdio.h>
#include <assert.h>

#include "../debug.h"

void test_mem()
{
    printf("\n%s\t\t", __func__);
}


int main() 
{
    setbuf(stdout, NULL);
    test_mem();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
