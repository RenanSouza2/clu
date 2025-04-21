#include <stdlib.h>

#include "../debug.h"
#include "../../../mem/header.h"

#include "../../../../mods/macros/assert.h"
#include "../../../../mods/macros/test.h"
#include "../../../../mods/macros/U64.h"



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
    printf("\n\t%s\t\t", __func__);

    #define TEST_LIST_BODY_CREATE(TAG, HANDLER)             \
    {                                                       \
        if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);  \
        list_body_p lb = clu_list_body_create(HANDLER);     \
        assert(lb->h == HANDLER);                           \
        for(uint64_t i=0; i<16; i++)                        \
            assert(lb->arr[i] == NULL);                     \
        FREE(lb, list_body);                                \
    }

    TEST_LIST_BODY_CREATE(1, NULL);
    TEST_LIST_BODY_CREATE(2, HD(1));

    #undef TEST_LIST_BODY_CREATE

    assert(clu_mem_internal_empty());
}

void test_list_body_create_immed_tree(bool show)
{
    printf("\n\t%s\t\t", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    list_body_p lb = clu_list_body_create_immed_tree(false);
    assert(lb == NULL);

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    lb = clu_list_body_create_immed_tree(true, HD(1));
    assert(lb != NULL);
    assert(lb->h == HD(1));
    clu_list_body_free(lb);

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    lb = clu_list_body_create_immed_tree(true, NULL, 2,
        1, HD(1),
        2, HD(2)
    );
    assert(lb != NULL);
    assert(lb->h == NULL);
    assert(lb->arr[1] != NULL);
    assert(lb->arr[1]->h == HD(1));
    assert(lb->arr[2] != NULL);
    assert(lb->arr[2]->h == HD(2));
    clu_list_body_free(lb);

    if(show) printf("\n\t\t%s 4\t\t", __func__);
    lb = clu_list_body_create_immed_tree(true, NULL, 1,
        0, NULL, 2,
            1, HD(0x10),
            2, HD(0x20)
    );
    assert(lb != NULL);
    assert(lb->h == NULL);
    assert(lb->arr[0] != NULL);
    assert(lb->arr[0]->arr[1] != NULL);
    assert(lb->arr[0]->arr[1]->h == HD(0x10));
    assert(lb->arr[0]->arr[2] != NULL);
    assert(lb->arr[0]->arr[2]->h == HD(0x20));
    clu_list_body_free(lb);

    assert(clu_mem_internal_empty());
}



void test_list_body_insert(bool show)
{
    printf("\n\t%s", __func__);

    #define TEST_LIST_BODY_INSERT(TAG, HANDLER, RES, ...)           \
    {                                                               \
        list_body_p lb[2];                                          \
        if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);          \
        clu_list_body_create_vec_immed_tree(lb, 2, __VA_ARGS__);    \
        bool res = clu_list_body_insert(&lb[0], HANDLER);           \
        assert(res == RES);                                         \
        assert(clu_list_body_str(lb[0], lb[1]));                    \
    }

    TEST_LIST_BODY_INSERT(1, HD(1), true,
        false,
        true, HD(1)
    );
    TEST_LIST_BODY_INSERT(2, HD(1), false,
        true, HD(1),
        true, HD(1)
    );
    TEST_LIST_BODY_INSERT(3, HD(2), true,
        true, HD(1),
        true, NULL, 2,
            1, HD(1),
            2, HD(2)
    );
    TEST_LIST_BODY_INSERT(4, HD(0x211), true,
        true, HD(0x111),
        true, NULL, 1,
            1, NULL, 1,
                1, NULL, 2,
                    1, HD(0x111),
                    2, HD(0x211)
    );

    #undef TEST_LIST_BODY_INSERT

    if(show) printf("\n\t\t%s 4\t\t", __func__);
    TEST_REVERT_OPEN
    clu_list_body_insert(NULL, HD(1));
    TEST_REVERT_CLOSE

    if(show) printf("\n\t\t%s 5\t\t", __func__);
    list_body_p lb = clu_list_body_create_immed_tree(false);
    TEST_REVERT_OPEN
    clu_list_body_insert(&lb, NULL);
    TEST_REVERT_CLOSE

    assert(clu_mem_internal_empty());
}

void test_list_body_remove(bool show)
{
    printf("\n\t%s\t\t", __func__);

    #define TEST_LIST_BODY_REMOVE(TAG, HANDLER, RES, ...)           \
    {                                                               \
        list_body_p lb[2];                                          \
        if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);          \
        clu_list_body_create_vec_immed_tree(lb, 2, __VA_ARGS__);    \
        bool res = clu_list_body_remove(&lb[0], HANDLER);           \
        assert(res == RES);                                         \
        assert(clu_list_body_str(lb[0], lb[1]));                    \
    }

    TEST_LIST_BODY_REMOVE(1, HD(1), false,
        false,
        false
    );
    TEST_LIST_BODY_REMOVE(2, HD(1), true,
        true, HD(1),
        false
    );
    TEST_LIST_BODY_REMOVE(3, HD(2), false,
        true, HD(1),
        true, HD(1)
    );
    TEST_LIST_BODY_REMOVE(4, HD(2), true,
        true, NULL, 2,
            1, HD(1),
            2, HD(2),
        true, NULL, 1,
            1, HD(1)
    );
    TEST_LIST_BODY_REMOVE(5, HD(1), true,
        true, NULL, 1,
            1, HD(1),
        false
    );
    TEST_LIST_BODY_REMOVE(6, HD(0x11), true,
        true, NULL, 1,
            1, NULL, 2,
                1, HD(0x11),
                2, HD(0x21),
        true, NULL, 1,
            1, NULL, 1,
                2, HD(0x21)
    );
    TEST_LIST_BODY_REMOVE(7, HD(1), false,
        true, NULL, 1,
            1, NULL, 1,
                2, HD(0x21),
        true, NULL, 1,
            1, NULL, 1,
                2, HD(0x21)
    );

    #undef TEST_LIST_BODY_REMOVE

    if(show) printf("\n\t\t%s 8\t\t", __func__);
    TEST_REVERT_OPEN
    clu_list_body_remove(NULL, HD(1));
    TEST_REVERT_CLOSE

    if(show) printf("\n\t\t%s 9\t\t", __func__);
    list_body_p lb = clu_list_body_create_immed_tree(0);
    TEST_REVERT_OPEN
    clu_list_body_remove(&lb, NULL);
    TEST_REVERT_CLOSE

    assert(clu_mem_internal_empty());
}



void test_list_body_count(bool show)
{
    printf("\n\t%s\t\t", __func__);

    #define TEST_LIST_BODY_COUNT(TAG, RES, ...)                         \
    {                                                                   \
        if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);              \
        list_body_p lb = clu_list_body_create_immed_tree(__VA_ARGS__);  \
        uint64_t res = clu_list_body_count(lb);                         \
        assert(uint64(res, RES));                                       \
        clu_list_body_free(lb);                                         \
    }

    TEST_LIST_BODY_COUNT(1, 0,
        false
    );
    TEST_LIST_BODY_COUNT(2, 1,
        true, HD(1)
    );
    TEST_LIST_BODY_COUNT(3, 1,
        true, NULL, 1,
            1, HD(1)
    );
    TEST_LIST_BODY_COUNT(4, 2,
        true, NULL, 2,
            1, HD(1),
            2, HD(2)
    );

    #undef TEST_LIST_BODY_COUNT

    assert(clu_mem_internal_empty());
}

void test_list_body_get_handler(bool show)
{
    printf("\n\t%s\t\t", __func__);

    #define TEST_LIST_BODY_GET_HANDLER(TAG, INDEX, RES, ...)            \
    {                                                                   \
        if(show) printf("\n\t\t%s %2d\t\t", __func__, TAG);             \
        list_body_p lb = clu_list_body_create_immed_tree(__VA_ARGS__);  \
        handler_p h = clu_list_body_get_handler(lb, INDEX);             \
        assert(h == RES);                                               \
        clu_list_body_free(lb);                                         \
    }

    TEST_LIST_BODY_GET_HANDLER(1, 0, HD(1),
        true, HD(1)
    );
    TEST_LIST_BODY_GET_HANDLER(2, 1, NULL,
        true, HD(1)
    );
    TEST_LIST_BODY_GET_HANDLER(3, 2, NULL,
        true, HD(1)
    );
    TEST_LIST_BODY_GET_HANDLER(4, 0, HD(1),
        true, NULL, 1,
            1, HD(1)
    );
    TEST_LIST_BODY_GET_HANDLER(5, 1, NULL,
        true, NULL, 1,
            1, HD(1)
    );
    TEST_LIST_BODY_GET_HANDLER(6, 2, NULL,
        true, NULL, 1,
            1, HD(1)
    );
    TEST_LIST_BODY_GET_HANDLER(7, 0, HD(1),
        true, NULL, 2,
            1, HD(1),
            2, HD(2)
    );
    TEST_LIST_BODY_GET_HANDLER(8, 1, HD(2),
        true, NULL, 2,
            1, HD(1),
            2, HD(2)
    );
    TEST_LIST_BODY_GET_HANDLER(9, 2, NULL,
        true, NULL, 2,
            1, HD(1),
            2, HD(2)
    );
    TEST_LIST_BODY_GET_HANDLER(10, 3, NULL,
        true, NULL, 2,
            1, HD(1),
            2, HD(2)
    );

    #undef TEST_LIST_BODY_GET_HANDLER

    if(show) printf("\n\t\t%s 11\t\t", __func__);
    TEST_REVERT_OPEN
    clu_list_body_get_handler(NULL, 0);
    TEST_REVERT_CLOSE

    assert(clu_mem_internal_empty());
}

void test_list_body_contains(bool show)
{

    printf("\n\t%s\t\t", __func__);

    #define TEST_LIST_BODY_CONTANS(TAG, HANDLER, RES, ...)              \
    {                                                                   \
        if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);              \
        list_body_p lb = clu_list_body_create_immed_tree(__VA_ARGS__);  \
        bool res = clu_list_body_contains(lb, HANDLER);                 \
        assert(res == RES);                                             \
        clu_list_body_free(lb);                                         \
    }

    TEST_LIST_BODY_CONTANS(1, HD(1), true,
        true, HD(1)
    );
    TEST_LIST_BODY_CONTANS(2, HD(2), false,
        true, HD(1)
    );
    TEST_LIST_BODY_CONTANS(3, HD(1), true,
        true, NULL, 2,
            1, HD(1),
            2, HD(2)
    );
    TEST_LIST_BODY_CONTANS(4, HD(2), true,
        true, NULL, 2,
            1, HD(1),
            2, HD(2)
    );
    TEST_LIST_BODY_CONTANS(5, HD(3), false,
        true, NULL, 2,
            1, HD(1),
            2, HD(2)
    );

    #undef TEST_LIST_BODY_CONTANS

    #define TEST_LIST_BODY_CONTANS(TAG, HANDLER, ...)                   \
    {                                                                   \
        if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);              \
        list_body_p lb = clu_list_body_create_immed_tree(__VA_ARGS__);  \
        TEST_REVERT_OPEN                                                \
        clu_list_body_contains(lb, HANDLER);                            \
        TEST_REVERT_CLOSE                                               \
        clu_list_body_free(lb);                                         \
    }

    TEST_LIST_BODY_CONTANS(6, HD(1),
        false
    );
    TEST_LIST_BODY_CONTANS(7, NULL,
        true, HD(1)
    );

    #undef TEST_LIST_BODY_CONTANS

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
    test_list_body_create_immed_tree(show);

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
    TEST_TIMEOUT_OPEN(TEST_TIMEOUT_DEFAULT)
    test_list_body();
    TEST_TIMEOUT_CLOSE
    printf("\n\n\tTest successful\n\n");
    return 0;
}
