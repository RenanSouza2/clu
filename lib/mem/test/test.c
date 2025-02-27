#include <stdio.h>

#include "../header.h"



void test_list_mem()
{
    printf("\n%s", __func__);
}


int main()
{
    setbuf(stdout, NULL);
    test_list_mem();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
