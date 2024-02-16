#include <stdio.h>
#include <stdlib.h>

#include "../debug.h"
#include "../../mem/header.h"
#include "../../../utils/assert.h"

void test_list_body_create()
{
    printf("\n\t%s", __func__);
    
    list_body_p lb = clu_list_body_create(HD(1));
    assert(lb->h  == HD(1));
    assert(lb->lb == NULL);
    free(lb, list_body);

    assert(clu_mem_empty());
}

void test_list_body_pop()
{
    printf("\n\t%s", __func__);

    list_body_p lb = clu_list_body_create(HD(1));
    lb->lb = LB(1);
    lb = clu_list_body_pop(lb);
    assert(lb == LB(1));

    assert(clu_mem_empty());
}

void test_list_body_insert()
{
    printf("\n\t%s", __func__);

    list_body_p lb = NULL;
    assert(clu_list_body_insert(&lb, HD(1)));
    assert(clu_list_body(lb, 1, HD(1)));

    assert(clu_list_body_insert(&lb, HD(1)) == false);
    
    assert(clu_list_body_insert(&lb, HD(2)));
    assert(clu_list_body(lb, 2, HD(1), HD(2)));
    clu_list_body_free(lb);

    assert(clu_mem_empty());
}

void test_list_body_remove()
{
    printf("\n\t%s", __func__);

    list_body_p lb = clu_list_body_create(HD(1));
    
    assert(clu_list_body_remove(&lb, HD(2)) == false);
    assert(clu_list_body(lb, 1, HD(1)));

    assert(clu_list_body_remove(&lb, (handler_p)1) == true);
    assert(lb == NULL);

    assert(clu_list_body_remove(&lb, (handler_p)1) == false);

    assert(clu_mem_empty());
}

void test_list_body()
{
    printf("\n%s", __func__);

    test_list_body_create();
    test_list_body_pop();

    test_list_body_insert();
    test_list_body_remove();

    assert(clu_mem_empty());
}

int main() 
{
    setbuf(stdout, NULL);
    test_list_body();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
