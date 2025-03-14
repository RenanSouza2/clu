#include <stdlib.h>

#include "../debug.h"
#include "../../../mem/header.h"
#include "../../../../utils/U64.h"
#include "../../../../utils/assert.h"
#include "../../../../utils/test_revert.h"



void test_offset(bool show)
{
    printf("\n\t%s", __func__);

    #define TEST_OFFSET(TAG, IN, RES)   \
    {   \
        if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);  \
        uint64_t res = OFFSET(IN);  \
        assert(uint64(res, RES));   \
    }

    TEST_OFFSET(1, 15, 60);
    TEST_OFFSET(2, 0, 0);
    TEST_OFFSET(3, 5, 20);

    #undef TEST_OFFSET
}

void test_get(bool show)
{
    printf("\n\t%s", __func__);

    handler_p h = HD(0xfedcba9876543210);

    for(uint64_t index=0; index<16; index++)
    {
        if(show) printf("\n\t\t%s " U64P(2) "\t\t", __func__, index + 1);
        uint64_t key = GET(h, index);
        assert(uint64(key, index));
    }

    assert(clu_mem_internal_empty());
}

void test_set(bool show)
{
    printf("\n\t%s", __func__);


    if(show) printf("\n\t\t%s 1\t\t", __func__);
    handler_p h = HD(0);
    handler_p h_res = SET(h, 15, 0xa);
    assert(h_res == HD(0xa000000000000000));

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    h = HD(0);
    h_res = SET(h, 0, 0xb);
    assert(h_res == HD(0xb));

    assert(clu_mem_internal_empty());
}

void test_list_body_create(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    list_body_p lb = clu_list_body_create(HD(1));
    assert(lb->h == HD(1));
    for(uint64_t i=0; i<16; i++)
        assert(lb->arr[i] == NULL);

    free(lb, list_body);

    assert(clu_mem_internal_empty());
}

void test_list_body_insert(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    list_body_p lb = clu_list_body_create_immed(0);
    bool res = clu_list_body_insert(&lb, HD(1));
    assert(res == true);
    assert(clu_list_body_immed(lb, 1, HD(1)));

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    lb = clu_list_body_create_immed(1, HD(1));
    res = clu_list_body_insert(&lb, HD(2));
    assert(res == true);
    assert(clu_list_body_immed(lb, 2, HD(1), HD(2)));

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    lb = clu_list_body_create_immed(1, HD(1));
    res = clu_list_body_insert(&lb, HD(1));
    assert(res == false);
    assert(clu_list_body_immed(lb, 1, HD(1)));

    if(show) printf("\n\t\t%s 4\t\t", __func__);
    TEST_REVERT_OPEN
    clu_list_body_insert(&lb, HD(1));
    TEST_REVERT_CLOSE

    if(show) printf("\n\t\t%s 4\t\t", __func__);
    lb = clu_list_body_create_immed(0);
    TEST_REVERT_OPEN
    clu_list_body_insert(&lb, NULL);
    TEST_REVERT_CLOSE

    assert(clu_mem_internal_empty());
}

void test_list_body_remove(bool show)
{
    printf("\n\t%s\t\t", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    list_body_p lb = clu_list_body_create_immed(0);
    bool res = clu_list_body_remove(&lb, HD(1));
    assert(res == false);
    assert(clu_list_body_immed(lb, 0));

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    lb = clu_list_body_create_immed(1, HD(1));
    res = clu_list_body_remove(&lb, HD(2));
    assert(res == false);
    assert(clu_list_body_immed(lb, 1, HD(1)));

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    lb = clu_list_body_create_immed(1, HD(1));
    res = clu_list_body_remove(&lb, HD(1));
    assert(res == true);
    assert(clu_list_body_immed(lb, 0));

    if(show) printf("\n\t\t%s 4\t\t", __func__);
    lb = clu_list_body_create_immed(2, HD(1), HD(2));
    res = clu_list_body_remove(&lb, HD(1));
    assert(res == true);
    assert(clu_list_body_immed(lb, 1, HD(2)));

    if(show) printf("\n\t\t%s 5\t\t", __func__);
    TEST_REVERT_OPEN
    clu_list_body_remove(NULL, HD(1));
    TEST_REVERT_CLOSE

    if(show) printf("\n\t\t%s 4\t\t", __func__);
    lb = clu_list_body_create_immed(0);
    TEST_REVERT_OPEN
    clu_list_body_remove(&lb, NULL);
    TEST_REVERT_CLOSE

    assert(clu_mem_internal_empty());
}



void test_list_body_count(bool show)
{
    printf("\n\t%s\t\t", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    list_body_p lb = clu_list_body_create_immed(0);
    uint64_t res = clu_list_body_count(lb);
    assert(uint64(res, 0));
    clu_list_body_free(lb);

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    lb = clu_list_body_create_immed(1, HD(1));
    res = clu_list_body_count(lb);
    assert(uint64(res, 1));
    clu_list_body_free(lb);

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    lb = clu_list_body_create_immed(2, HD(1), HD(2));
    res = clu_list_body_count(lb);
    assert(uint64(res, 2));
    clu_list_body_free(lb);

    assert(clu_mem_internal_empty());
}



void test_list_body_get_handler(bool show)
{
    printf("\n\t%s\t\t", __func__);

    #define TEST_LIST_BODY_GET_HANDLER(TAG, INDEX, RES, ...)        \
    {                                                               \
        if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);          \
        list_body_p lb = clu_list_body_create_immed(__VA_ARGS__);   \
        handler_p h = clu_list_body_get_handler(lb, INDEX);         \
        assert(h == RES);                                           \
        clu_list_body_free(lb);                                     \
    }

    TEST_LIST_BODY_GET_HANDLER(1, 0, HD(1), 1, HD(1));
    TEST_LIST_BODY_GET_HANDLER(2, 1, NULL, 1, HD(1));
    TEST_LIST_BODY_GET_HANDLER(3, 0, HD(1), 2, HD(1), HD(2));
    TEST_LIST_BODY_GET_HANDLER(4, 1, HD(2), 2, HD(1), HD(2));
    TEST_LIST_BODY_GET_HANDLER(5, 2, NULL, 2, HD(1), HD(2));
    TEST_LIST_BODY_GET_HANDLER(6, 3, NULL, 2, HD(1), HD(2));

    #undef TEST_LIST_BODY_GET_HANDLER

    if(show) printf("\n\t\t%s 7\t\t", __func__);
    list_body_p lb = clu_list_body_create_immed(2, HD(1), HD(2));
    assert(clu_list_body_remove(&lb, HD(2)));
    handler_p h = clu_list_body_get_handler(lb, 0);
    assert(h == HD(1));
    clu_list_body_free(lb);

    if(show) printf("\n\t\t%s 8\t\t", __func__);
    TEST_REVERT_OPEN
    clu_list_body_get_handler(NULL, 0);
    TEST_REVERT_CLOSE

    assert(clu_mem_internal_empty());
}

void test_list_body_contains(bool show)
{

    printf("\n\t%s\t\t", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    list_body_p lb = clu_list_body_create_immed(1, HD(1));
    bool res = clu_list_body_contains(lb, HD(1));
    assert(res == true);
    clu_list_body_free(lb);

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    lb = clu_list_body_create_immed(1, HD(1));
    res = clu_list_body_contains(lb, HD(2));
    assert(res == false);
    clu_list_body_free(lb);

    assert(clu_mem_internal_empty());
}



void test_list_body()
{
    printf("\n%s", __func__);

    bool show = false;

    test_offset(show);
    test_get(show);
    test_set(show);

    test_list_body_create(show);
    test_list_body_insert(show);
    test_list_body_remove(show);

    test_list_body_count(show);
    test_list_body_get_handler(show);
    test_list_body_contains(show);

    assert(clu_mem_internal_empty());
}



int main()
{
    setbuf(stdout, NULL);
    test_list_body();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
