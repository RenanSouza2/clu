#include <stdio.h>
#include <assert.h>

#include "../debug.h"

void test_list_head()
{
    printf("\n%s\t\t", __func__);
}


int main() 
{
    setbuf(stdout, NULL);
    test_list_head();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
