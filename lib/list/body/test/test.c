#include "../debug.h"
#include "../../../../testrc.h"
#include "../../../../mods/macros/test.h"

#include "../../../../mods/macros/U64.h"



void test_offset(bool show)
{
    TEST_FN_OPEN

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

    TEST_FN_CLOSE
}

void test_get(bool show)
{
    TEST_FN_OPEN

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

    TEST_FN_CLOSE
}

void test_set(bool show)
{
    TEST_FN_OPEN

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

    TEST_FN_CLOSE
}

void test_trie_create(bool show)
{
    TEST_FN_OPEN

    #define TEST_TRIE_CREATE(TAG, HANDLER)          \
    {                                               \
        TEST_CASE_OPEN(TAG)                         \
        {                                           \
            trie_p t = clu_trie_create(HANDLER);    \
            assert(t->h == HANDLER);                \
            for(uint64_t i=0; i<16; i++)            \
            assert(t->arr[i] == NULL);              \
            FREE(t, trie);                          \
        }                                           \
        TEST_CASE_CLOSE                             \
    }

    TEST_TRIE_CREATE(1, NULL);
    TEST_TRIE_CREATE(2, HD(1));

    #undef TEST_TRIE_CREATE

    TEST_FN_CLOSE
}

void test_trie_create_immed_tree(bool show)
{
    TEST_FN_OPEN

    TEST_CASE_OPEN(1)
    {
        trie_p t = clu_trie_create_immed_tree(false);
        assert(t == NULL);
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(2)
    {
        trie_p t = clu_trie_create_immed_tree(true, HD(1));
        assert(t != NULL);
        assert(t->h == HD(1));
        clu_trie_free(t);
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(3)
    {
        trie_p t = clu_trie_create_immed_tree(true, NULL, 2,
            1, HD(1),
            2, HD(2)
        );
        assert(t != NULL);
        assert(t->h == NULL);
        assert(t->arr[1] != NULL);
        assert(t->arr[1]->h == HD(1));
        assert(t->arr[2] != NULL);
        assert(t->arr[2]->h == HD(2));
        clu_trie_free(t);
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(3)
    {
        trie_p t = clu_trie_create_immed_tree(true, NULL, 1,
            0, NULL, 2,
                1, HD(0x10),
                2, HD(0x20)
        );
        assert(t != NULL);
        assert(t->h == NULL);
        assert(t->arr[0] != NULL);
        assert(t->arr[0]->arr[1] != NULL);
        assert(t->arr[0]->arr[1]->h == HD(0x10));
        assert(t->arr[0]->arr[2] != NULL);
        assert(t->arr[0]->arr[2]->h == HD(0x20));
        clu_trie_free(t);
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}



void test_trie_insert(bool show)
{
    TEST_FN_OPEN

    #define TEST_TRIE_INSERT(TAG, T_BEF, HANDLER, RES, T_AFT)       \
    {                                                               \
        TEST_CASE_OPEN(TAG)                                         \
        {                                                           \
            trie_p t = clu_trie_create_immed_tree(ARG_OPEN T_BEF);  \
            bool res = clu_trie_insert(&t, HANDLER);                \
            assert(res == RES);                                     \
            assert(clu_trie_immed_tree(t, ARG_OPEN T_AFT));         \
        }                                                           \
        TEST_CASE_CLOSE                                             \
    }

    TEST_TRIE_INSERT(1,
        (false),
        HD(1),
        true,
        (true, HD(1))
    );
    TEST_TRIE_INSERT(2,
        (true, HD(1)),
        HD(1),
        false,
        (true, HD(1))
    );
    TEST_TRIE_INSERT(3,
        (true, HD(1)),
        HD(2),
        true,
        (
            true, NULL, 2,
                1, HD(1),
                2, HD(2)
        )
    );
    TEST_TRIE_INSERT(4,
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

    #undef TEST_TRIE_INSERT

    TEST_CASE_OPEN(4)
    {
        TEST_REVERT_OPEN
        {
            clu_trie_insert(NULL, HD(1));
        }
        TEST_REVERT_CLOSE
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(5)
    {
        if(show) printf("\n\t\t%s 5\t\t", __func__);
        trie_p t = clu_trie_create_immed_tree(false);
        TEST_REVERT_OPEN
        {
            clu_trie_insert(&t, NULL);
        }
        TEST_REVERT_CLOSE
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}

void test_trie_remove(bool show)
{
    TEST_FN_OPEN

    #define TEST_TRIE_REMOVE(TAG, T_BEF, HANDLER, RES, T_AFT)       \
    {                                                               \
        TEST_CASE_OPEN(TAG)                                         \
        {                                                           \
            trie_p t = clu_trie_create_immed_tree(ARG_OPEN T_BEF);  \
            bool res = clu_trie_remove(&t, HANDLER);                \
            assert(res == RES);                                     \
            assert(clu_trie_immed_tree(t, ARG_OPEN T_AFT));         \
        }                                                           \
        TEST_CASE_CLOSE                                             \
    }

    TEST_TRIE_REMOVE(1,
        (true, HD(1)),
        HD(1),
        true,
        (false)
    );
    TEST_TRIE_REMOVE(2,
        (true, HD(1)),
        HD(2),
        false,
        (true, HD(1))
    );
    TEST_TRIE_REMOVE(3,
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
    TEST_TRIE_REMOVE(4,
        (
            true, NULL, 1,
                1, HD(1)
        ),
        HD(1),
        true,
        (false)
    );
    TEST_TRIE_REMOVE(5,
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
    TEST_TRIE_REMOVE(6,
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

    #undef TEST_TRIE_REMOVE

    TEST_CASE_OPEN(7)
    {
        TEST_REVERT_OPEN
        {
            clu_trie_remove(NULL, HD(1));
        }
        TEST_REVERT_CLOSE
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(8)
    {
        trie_p t = clu_trie_create_immed_tree(false);
        TEST_REVERT_OPEN
        {
            clu_trie_remove(&t, HD(1));
        }
        TEST_REVERT_CLOSE
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(9)
    {
        trie_p t = clu_trie_create_immed_tree(false);
        TEST_REVERT_OPEN
        {
            clu_trie_remove(&t, NULL);
        }
        TEST_REVERT_CLOSE
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}



void test_trie_count(bool show)
{
    TEST_FN_OPEN

    #define TEST_TRIE_COUNT(TAG, RES, ...)                          \
    {                                                               \
        TEST_CASE_OPEN(TAG)                                         \
        {                                                           \
            trie_p t = clu_trie_create_immed_tree(__VA_ARGS__);     \
            uint64_t res = clu_trie_count(t);                       \
            assert(uint64(res, RES));                               \
            clu_trie_free(t);                                       \
        }                                                           \
        TEST_CASE_CLOSE                                             \
    }

    TEST_TRIE_COUNT(1, 0,
        false
    );
    TEST_TRIE_COUNT(2, 1,
        true, HD(1)
    );
    TEST_TRIE_COUNT(3, 1,
        true, NULL, 1,
            1, HD(1)
    );
    TEST_TRIE_COUNT(4, 2,
        true, NULL, 2,
            1, HD(1),
            2, HD(2)
    );

    #undef TEST_TRIE_COUNT

    TEST_FN_CLOSE
}

void test_trie_get_handler(bool show)
{
    TEST_FN_OPEN

    #define TEST_TRIE_GET_HANDLER(TAG, INDEX, RES, ...)             \
    {                                                               \
        TEST_CASE_OPEN(TAG)                                         \
        {                                                           \
            trie_p t = clu_trie_create_immed_tree(__VA_ARGS__);     \
            handler_p h = clu_trie_get_handler(t, INDEX);           \
            assert(h == RES);                                       \
            clu_trie_free(t);                                       \
        }                                                           \
        TEST_CASE_CLOSE                                             \
    }

    TEST_TRIE_GET_HANDLER(1, 0, HD(1),
        true, HD(1)
    );
    TEST_TRIE_GET_HANDLER(2, 1, NULL,
        true, HD(1)
    );
    TEST_TRIE_GET_HANDLER(3, 2, NULL,
        true, HD(1)
    );
    TEST_TRIE_GET_HANDLER(4, 0, HD(1),
        true, NULL, 1,
            1, HD(1)
    );
    TEST_TRIE_GET_HANDLER(5, 1, NULL,
        true, NULL, 1,
            1, HD(1)
    );
    TEST_TRIE_GET_HANDLER(6, 2, NULL,
        true, NULL, 1,
            1, HD(1)
    );
    TEST_TRIE_GET_HANDLER(7, 0, HD(1),
        true, NULL, 2,
            1, HD(1),
            2, HD(2)
    );
    TEST_TRIE_GET_HANDLER(8, 1, HD(2),
        true, NULL, 2,
            1, HD(1),
            2, HD(2)
    );
    TEST_TRIE_GET_HANDLER(9, 2, NULL,
        true, NULL, 2,
            1, HD(1),
            2, HD(2)
    );
    TEST_TRIE_GET_HANDLER(10, 3, NULL,
        true, NULL, 2,
            1, HD(1),
            2, HD(2)
    );

    #undef TEST_TRIE_GET_HANDLER

    TEST_CASE_OPEN(11)
    {
        TEST_REVERT_OPEN
        {
            clu_trie_get_handler(NULL, 0);
        }
        TEST_REVERT_CLOSE
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}

void test_trie_contains(bool show)
{
    TEST_FN_OPEN

    #define TEST_TRIE_CONTANS(TAG, HANDLER, RES, ...)               \
    {                                                               \
        TEST_CASE_OPEN(TAG)                                         \
        {                                                           \
            trie_p t = clu_trie_create_immed_tree(__VA_ARGS__);     \
            bool res = clu_trie_contains(t, HANDLER);               \
            assert(res == RES);                                     \
            clu_trie_free(t);                                       \
        }                                                           \
        TEST_CASE_CLOSE                                             \
    }

    TEST_TRIE_CONTANS(1, HD(1), true,
        true, HD(1)
    );
    TEST_TRIE_CONTANS(2, HD(2), false,
        true, HD(1)
    );
    TEST_TRIE_CONTANS(3, HD(1), true,
        true, NULL, 2,
            1, HD(1),
            2, HD(2)
    );
    TEST_TRIE_CONTANS(4, HD(2), true,
        true, NULL, 2,
            1, HD(1),
            2, HD(2)
    );
    TEST_TRIE_CONTANS(5, HD(3), false,
        true, NULL, 2,
            1, HD(1),
            2, HD(2)
    );

    #undef TEST_TRIE_CONTANS

    #define TEST_TRIE_CONTANS(TAG, HANDLER, ...)                    \
    {                                                               \
        TEST_CASE_OPEN(TAG)                                         \
        {                                                           \
            trie_p t = clu_trie_create_immed_tree(__VA_ARGS__);     \
            TEST_REVERT_OPEN                                        \
            {                                                       \
                clu_trie_contains(t, HANDLER);                      \
            }                                                       \
            TEST_REVERT_CLOSE                                       \
        }                                                           \
        TEST_CASE_CLOSE                                             \
    }

    TEST_TRIE_CONTANS(6, HD(1),
        false
    );
    TEST_TRIE_CONTANS(7, NULL,
        true, HD(1)
    );

    #undef TEST_TRIE_CONTANS

    TEST_FN_CLOSE
}



void test_trie()
{
    TEST_LIB

    bool show = false;

    test_offset(show);
    test_get(show);
    test_set(show);

    test_trie_create(show);
    test_trie_create_immed_tree(show);

    test_trie_insert(show);
    test_trie_remove(show);

    test_trie_count(show);
    test_trie_get_handler(show);
    test_trie_contains(show);

    TEST_ASSERT_MEM_EMPTY
}



int main()
{
    setbuf(stdout, NULL);
    test_trie();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
