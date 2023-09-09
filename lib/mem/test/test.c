#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../debug.h"

void test_create_body()
{
    printf("\n\ttest create body\t\t");
    
    list_body_p lb = list_body_create((handler_p)1, (list_body_p)2);
    assert(lb->h  == (handler_p)1);
    assert(lb->lb == (list_body_p)2);
    free(lb, list_body);
}

void test_create_head()
{
    printf("\n\ttest create head\t\t");

    list_head_p lh = list_head_create("test", (list_head_p)1);
    assert(lh->lh == (list_head_p)1);
    assert(lh->lb == NULL);
    assert(strcmp(lh->str, "test") == 0);
    free(lh, list_head);

    lh = list_head_create("", NULL);
    assert(strcmp(lh->str, "") == 0);
    free(lh, list_head);

    lh = list_head_create("abcdefghi abcdefghi abcdefghi abcdefghi abcdefghi", NULL);
    assert(strcmp(lh->str, "abcdefghi abcdefghi abcdefghi abcdefghi abcdefghi") == 0);
    free(lh, list_head);

    assert(list_memory());
}

void test_find()
{
    printf("\n\ttest find\t\t");

    list_head_p lh = NULL;

    list_head_p lh_res = list_head_find(lh, "test");
    assert(lh_res == NULL);

    lh = list_head_create("test", NULL);
    lh_res = list_head_find(lh, "test");
    assert(lh = lh_res);

    free(lh, list_head);
}

void test_mem()
{
    printf("\ntest mem library\t\t");

    test_create_body();
    test_create_head();
    test_find();

    assert(list_memory());
}


int main() 
{
    setbuf(stdout, NULL);
    test_mem();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
