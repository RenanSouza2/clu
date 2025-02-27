#include <stdlib.h>

#include "../debug.h"
#include "../../../mem/header.h"
#include "../../../../utils/assert.h"



void test_list_body_create(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    list_body_p lb = clu_list_body_create(HD(1));
    assert(lb->h  == HD(1));
    assert(lb->lb == NULL);
    free(lb, list_body);

    assert(clu_mem_internal_empty());
}

void test_list_body_create_immed(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    list_body_p lb = clu_list_body_create_immed(0);
    assert(lb == NULL);

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    lb = clu_list_body_create_immed(1, HD(1));
    assert(lb != NULL);
    assert(lb->h == HD(1));
    assert(lb->lb == NULL);
    free(lb, list_body);
    
    if(show) printf("\n\t\t%s 3\t\t", __func__);
    lb = clu_list_body_create_immed(2, HD(1), HD(2));
    assert(lb != NULL);
    assert(lb->h == HD(1));
    assert(lb->lb != NULL);
    assert(lb->lb->h == HD(2));
    assert(lb->lb->lb == NULL);
    free(lb->lb, list_body);
    free(lb, list_body);

    assert(clu_mem_internal_empty());
}

void test_list_body_pop(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    list_body_p lb = clu_list_body_create_immed(1, HD(1));
    lb = clu_list_body_pop(lb);
    assert(lb == NULL);

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    lb = clu_list_body_create_immed(2, HD(1), HD(2));
    lb = clu_list_body_pop(lb);
    assert(clu_list_body_immed(lb, 1, HD(2)));

    assert(clu_mem_internal_empty());
}

void test_list_body_insert(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    list_body_p lb = clu_list_body_create_immed(0);
    assert(clu_list_body_insert(&lb, HD(1)) == true);
    assert(clu_list_body_immed(lb, 1, HD(1)));

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    lb = clu_list_body_create_immed(1, HD(1));
    assert(clu_list_body_insert(&lb, HD(2)) == true);
    assert(clu_list_body_immed(lb, 2, HD(1), HD(2)));

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    lb = clu_list_body_create_immed(1, HD(1));
    assert(clu_list_body_insert(&lb, HD(1)) == false);
    assert(clu_list_body_immed(lb, 1, HD(1)));

    assert(clu_mem_internal_empty());
}

void test_list_body_remove(bool show)
{
    printf("\n\t%s\t\t", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    list_body_p lb = clu_list_body_create_immed(0);
    assert(clu_list_body_remove(&lb, HD(1)) == false);
    assert(clu_list_body_immed(lb, 0));
    
    if(show) printf("\n\t\t%s 2\t\t", __func__);
    lb = clu_list_body_create_immed(1, HD(1));
    assert(clu_list_body_remove(&lb, HD(2)) == false);
    assert(clu_list_body_immed(lb, 1, HD(1)));

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    lb = clu_list_body_create_immed(1, HD(1));
    assert(clu_list_body_remove(&lb, HD(1)) == true);
    assert(clu_list_body_immed(lb, 0));

    assert(clu_mem_internal_empty());
}



void test_list_body()
{
    printf("\n%s", __func__);

    bool show = true;

    test_list_body_create(show);
    test_list_body_create_immed(show);
    test_list_body_pop(show);
    test_list_body_insert(show);


    test_list_body_remove(show);

    assert(clu_mem_internal_empty());
}



int main()
{
    setbuf(stdout, NULL);
    test_list_body();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
