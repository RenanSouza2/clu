#include <stdlib.h>

#include "../debug.h"
#include "../../../mem/header.h"

#include "../../../../mods/macros/assert.h"
#include "../../../../mods/macros/test.h"
#include "../../../../mods/macros/U64.h"



#define TEST_ASSERT_EMPTY assert(clu_mem_internal_empty());

void test_offset(bool show)
{
    TEST_FN

    #define TEST_OFFSET(TAG, IN, RES)   \
    {                                   \
        TEST_CASE_OPEN(TAG)             \
        {                               \
            uint64_t res = OFFSET(IN);  \
            assert(uint64(res, RES));   \
        }                               \
        TEST_CASE_CLOSE;                \
    }

    TEST_OFFSET(1, 15, 60);
    TEST_OFFSET(2, 0, 0);
    TEST_OFFSET(3, 5, 20);

    #undef TEST_OFFSET
}

void test_get(bool show)
{
    TEST_FN

    handler_p h = HD(0xfedcba9876543210);

    for(uint64_t index=0; index<16; index++)
    {
        TEST_CASE_OPEN(index + 1)
        {
            uint64_t key = GET(h, index);
            assert(uint64(key, index));
        }
        TEST_CASE_CLOSE;
    }

    assert(clu_mem_internal_empty());
}

void test_set(bool show)
{
    TEST_FN

    TEST_CASE_OPEN(1)
    {
        handler_p h = HD(0);
        handler_p h_res = SET(h, 15, 0xa);
        assert(h_res == HD(0xa000000000000000));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(2)
    {
        handler_p h = HD(0);
        handler_p h_res = SET(h, 0, 0xb);
        assert(h_res == HD(0xb));
    }
    TEST_CASE_CLOSE

    assert(clu_mem_internal_empty());
}

void test_list_body_create(bool show)
{
    TEST_FN

    #define TEST_LIST_BODY_CREATE(TAG, HANDLER)             \
    {                                                       \
        TEST_CASE_OPEN(TAG)                                 \
        {                                                   \
            list_body_p lb = clu_list_body_create(HANDLER); \
            assert(lb->h == HANDLER);                       \
            for(uint64_t i=0; i<16; i++)                    \
            assert(lb->arr[i] == NULL);                     \
            FREE(lb, list_body);                            \
        }                                                   \
        TEST_CASE_CLOSE                                     \
    }

    TEST_LIST_BODY_CREATE(1, NULL);
    TEST_LIST_BODY_CREATE(2, HD(1));

    #undef TEST_LIST_BODY_CREATE

    assert(clu_mem_internal_empty());
}

void test_list_body_create_immed_tree(bool show)
{
    TEST_FN

    TEST_CASE_OPEN(1)
    {
        list_body_p lb = clu_list_body_create_immed_tree(false);
        assert(lb == NULL);
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(2)
    {
        list_body_p lb = clu_list_body_create_immed_tree(true, HD(1));
        assert(lb != NULL);
        assert(lb->h == HD(1));
        clu_list_body_free(lb);
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(3)
    {
        list_body_p lb = clu_list_body_create_immed_tree(true, NULL, 2,
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
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(3)
    {
        list_body_p lb = clu_list_body_create_immed_tree(true, NULL, 1,
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
    }
    TEST_CASE_CLOSE

    assert(clu_mem_internal_empty());
}



void test_list_body_insert(bool show)
{
    TEST_FN

    #define TEST_LIST_BODY_INSERT(TAG, lLB_BEF, HANDLER, RES, LB_AFT)           \
    {                                                                           \
        TEST_CASE_OPEN(TAG)                                                     \
        {                                                                       \
            list_body_p lb = clu_list_body_create_immed_tree(ARG_OPEN lLB_BEF); \
            bool res = clu_list_body_insert(&lb, HANDLER);                      \
            assert(res == RES);                                                 \
            assert(clu_list_body_immed_tree(lb, ARG_OPEN LB_AFT));              \
        }                                                                       \
        TEST_CASE_CLOSE                                                         \
    }

    TEST_LIST_BODY_INSERT(1,
        (false),
        HD(1),
        true,
        (true, HD(1))
    );
    TEST_LIST_BODY_INSERT(2,
        (true, HD(1)),
        HD(1),
        false,
        (true, HD(1))
    );
    TEST_LIST_BODY_INSERT(3,
        (true, HD(1)),
        HD(2),
        true,
        (
            true, NULL, 2,
                1, HD(1),
                2, HD(2)
        )
    );
    TEST_LIST_BODY_INSERT(4,
        (true, HD(0x111)),
        HD(0x211),
        true,
        (
            true, NULL, 1,
                1, NULL, 1,
                    1, NULL, 2,
                        1, HD(0x111),
                        2, HD(0x211)
        )
    );

    #undef TEST_LIST_BODY_INSERT

    TEST_CASE_OPEN(4)
    {
        TEST_REVERT_OPEN
        {
            clu_list_body_insert(NULL, HD(1));
        }
        TEST_REVERT_CLOSE
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(5)
    {
        if(show) printf("\n\t\t%s 5\t\t", __func__);
        list_body_p lb = clu_list_body_create_immed_tree(false);
        TEST_REVERT_OPEN
        {
            clu_list_body_insert(&lb, NULL);
        }
        TEST_REVERT_CLOSE
    }
    TEST_CASE_CLOSE

    assert(clu_mem_internal_empty());
}

void test_list_body_remove(bool show)
{
    TEST_FN

    #define TEST_LIST_BODY_REMOVE(TAG, LB_BEF, HANDLER, RES, LB_AFT)            \
    {                                                                           \
        TEST_CASE_OPEN(TAG)                                                     \
        {                                                                       \
            list_body_p lb = clu_list_body_create_immed_tree(ARG_OPEN LB_BEF);  \
            bool res = clu_list_body_remove(&lb, HANDLER);                      \
            assert(res == RES);                                                 \
            assert(clu_list_body_immed_tree(lb, ARG_OPEN LB_AFT));              \
        }                                                                       \
        TEST_CASE_CLOSE                                                         \
    }

    TEST_LIST_BODY_REMOVE(1,
        (true, HD(1)),
        HD(1),
        true,
        (false)
    );
    TEST_LIST_BODY_REMOVE(2,  
        (true, HD(1)),
        HD(2),
        false,
        (true, HD(1))
    );
    TEST_LIST_BODY_REMOVE(3,
        (
            true, NULL, 2,
                1, HD(1),
                2, HD(2)
        ),
        HD(2),
        true,
        (
            true, NULL, 1,
                1, HD(1)
        )
    );
    TEST_LIST_BODY_REMOVE(4,
        (
            true, NULL, 1,
                1, HD(1)
        ),
        HD(1),
        true,
        (false)
    );
    TEST_LIST_BODY_REMOVE(5,  
        (
            true, NULL, 1,
                1, NULL, 2,
                    1, HD(0x11),
                    2, HD(0x21)
        ),
        HD(0x11),
        true,
        (
            true, NULL, 1,
                1, NULL, 1,
                    2, HD(0x21)
        )
    );
    TEST_LIST_BODY_REMOVE(6,  
        (
            true, NULL, 1,
                1, NULL, 1,
                    2, HD(0x21)
        ),
        HD(1),
        false,
        (
            true, NULL, 1,
                1, NULL, 1,
                    2, HD(0x21)
        )
    );

    #undef TEST_LIST_BODY_REMOVE
    
    TEST_CASE_OPEN(7)
    {
        TEST_REVERT_OPEN
        {
            clu_list_body_remove(NULL, HD(1));
        }
        TEST_REVERT_CLOSE
    }
    TEST_CASE_CLOSE
    
    TEST_CASE_OPEN(8)
    {
        list_body_p lb = clu_list_body_create_immed_tree(false);
        TEST_REVERT_OPEN
        {
            clu_list_body_remove(&lb, HD(1));
        }
        TEST_REVERT_CLOSE
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(9)
    {
        list_body_p lb = clu_list_body_create_immed_tree(false);
        TEST_REVERT_OPEN
        {
            clu_list_body_remove(&lb, NULL);
        }
        TEST_REVERT_CLOSE
    }
    TEST_CASE_CLOSE

    assert(clu_mem_internal_empty());
}



void test_list_body_count(bool show)
{
    TEST_FN

    #define TEST_LIST_BODY_COUNT(TAG, RES, ...)                             \
    {                                                                       \
        TEST_CASE_OPEN(TAG)                                                 \
        {                                                                   \
            list_body_p lb = clu_list_body_create_immed_tree(__VA_ARGS__);  \
            uint64_t res = clu_list_body_count(lb);                         \
            assert(uint64(res, RES));                                       \
            clu_list_body_free(lb);                                         \
        }                                                                   \
        TEST_CASE_CLOSE                                                     \
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
    TEST_FN

    #define TEST_LIST_BODY_GET_HANDLER(TAG, INDEX, RES, ...)                \
    {                                                                       \
        TEST_CASE_OPEN(TAG)                                                 \
        {                                                                   \
            list_body_p lb = clu_list_body_create_immed_tree(__VA_ARGS__);  \
            handler_p h = clu_list_body_get_handler(lb, INDEX);             \
            assert(h == RES);                                               \
            clu_list_body_free(lb);                                         \
        }                                                                   \
        TEST_CASE_CLOSE                                                     \
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

    TEST_CASE_OPEN(11)
    {
        TEST_REVERT_OPEN
        {
            clu_list_body_get_handler(NULL, 0);
        }
        TEST_REVERT_CLOSE
    }
    TEST_CASE_CLOSE

    assert(clu_mem_internal_empty());
}

void test_list_body_contains(bool show)
{
    TEST_FN

    #define TEST_LIST_BODY_CONTANS(TAG, HANDLER, RES, ...)                  \
    {                                                                       \
        TEST_CASE_OPEN(TAG)                                                 \
        {                                                                   \
            list_body_p lb = clu_list_body_create_immed_tree(__VA_ARGS__);  \
            bool res = clu_list_body_contains(lb, HANDLER);                 \
            assert(res == RES);                                             \
            clu_list_body_free(lb);                                         \
        }                                                                   \
        TEST_CASE_CLOSE                                                     \
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

    #define TEST_LIST_BODY_CONTANS(TAG, HANDLER, ...)                       \
    {                                                                       \
        TEST_CASE_OPEN(TAG)                                                 \
        {                                                                   \
            list_body_p lb = clu_list_body_create_immed_tree(__VA_ARGS__);  \
            TEST_REVERT_OPEN                                                \
            {                                                               \
                clu_list_body_contains(lb, HANDLER);                        \
            }                                                               \
            TEST_REVERT_CLOSE                                               \
            clu_list_body_free(lb);                                         \
        }                                                                   \
        TEST_CASE_CLOSE                                                     \
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

    bool show = true;

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
