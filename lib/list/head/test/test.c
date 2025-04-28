#include "../debug.h"
#include "../../../../testrc.h"
#include "../../../../mods/macros/test.h"

#include "../../body/debug.h"
#include "../../../tag/debug.h"



void test_list_create(bool show)
{
    TEST_FN_OPEN

    tag_t tag = clu_tag_format("test");

    TEST_CASE_OPEN(1)
    {
        list_p l = clu_list_create(&tag, NULL);
        assert(clu_tag(&l->tag, &tag));
        assert(l->t == NULL);
        assert(l->next == NULL);
        FREE(l, list);
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(2)
    {
        list_p l = clu_list_create(&tag, L(1));
        assert(clu_tag(&l->tag, &tag));
        assert(l->t == NULL);
        assert(l->next == L(1));
        FREE(l, list);
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}

void test_list_pop(bool show)
{
    TEST_FN_OPEN

    tag_t tag_1 = clu_tag_format("test_1");
    tag_t tag_2 = clu_tag_format("test_2");

    TEST_CASE_OPEN(1)
    {
        list_p l = clu_list_create(&tag_1, NULL);
        l = clu_list_pop(l);
        assert(l == NULL);
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(2)
    {
        list_p l = clu_list_create(&tag_2, NULL);
        l = clu_list_create(&tag_1, l);
        l = clu_list_pop(l);
        assert(l != NULL);
        assert(clu_tag(&l->tag, &tag_2));
        FREE(l, list);
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(3)
    {
        TEST_REVERT_OPEN
        {
            clu_list_pop(NULL);
        }
        TEST_REVERT_CLOSE
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}

void test_list_create_variadic(bool show)
{
    TEST_FN_OPEN

    tag_t tag_1 = clu_tag_format("test 1");
    tag_t tag_2 = clu_tag_format("test 2");

    TEST_CASE_OPEN(1)
    {
        list_p l = clu_list_create_immed(0);
        assert(l == NULL);
        clu_list_free(l);
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(2)
    {
        list_p l = clu_list_create_immed(1,
            tag_1, 1, HD(1), 0
        );
        assert(l != NULL);
        assert(clu_trie_immed_list(l->t, 1, HD(1), 0))
        assert(l->next == NULL);
        FREE(l, list);
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(3)
    {
        list_p l = clu_list_create_immed(1,
            tag_1, 2, HD(1), HD(2), 0
        );
        assert(l != NULL);
        assert(clu_trie_immed_list(l->t, 2, HD(1), HD(2), 0))
        assert(l->next == NULL);
        FREE(l, list);
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(4)
    {
        list_p l = clu_list_create_immed(2,
            tag_1, 1, HD(1), 0,
            tag_2, 1, HD(2), 0
        );
        assert(l != NULL);
        assert(clu_trie_immed_list(l->t, 1, HD(1), 0));
        assert(l->next != NULL);
        assert(clu_trie_immed_list(l->next->t, 1, HD(2), 0));
        assert(l->next->next == NULL);
        FREE(l->next, list);
        FREE(l, list);
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}



void test_list_insert(bool show)
{
    TEST_FN_OPEN

    tag_t tag_1 = clu_tag_format("test 1");
    tag_t tag_2 = clu_tag_format("test 2");

    #define TEST_LIST_INSERT(TAG, L_BEF, LTAG, HANDLER, RES, L_AFT)  \
    {                                                                       \
        TEST_CASE_OPEN(TAG)                                                 \
        {                                                                   \
            list_p l = clu_list_create_immed(ARG_OPEN L_BEF);   \
            bool res = clu_list_insert(&l, &LTAG, HANDLER);           \
            assert(res == RES);                                             \
            assert(clu_list_immed(l, ARG_OPEN L_AFT));               \
        }                                                                   \
        TEST_CASE_CLOSE                                                     \
    }

    TEST_LIST_INSERT(1,
        (0),
        tag_1, HD(1),
        true,
        (1,  tag_1, 1, HD(1), 0)
    );
    TEST_LIST_INSERT(2,
        (1,  tag_1, 1, HD(1), 0),
        tag_1, HD(1),
        false,
        (1,  tag_1, 1, HD(1), 0)
    );
    TEST_LIST_INSERT(3,
        (1,  tag_1, 1, HD(1), 0),
        tag_2, HD(1),
        false,
        (1,  tag_1, 1, HD(1), 0)
    );
    TEST_LIST_INSERT(4,
        (1,  tag_1, 1, HD(1), 0),
        tag_1, HD(2),
        true,
        (1,  tag_1, 2, HD(1), HD(2), 0)
    );
    TEST_LIST_INSERT(5,
        (1,  tag_1, 2, HD(1), HD(2), 0),
        tag_2, HD(3),
        true,
        (
            2,  tag_2, 1, HD(3), 0,
                tag_1, 2, HD(1), HD(2), 0
        )
    );
    TEST_LIST_INSERT(6,
        (
            2,  tag_1, 2, HD(1), HD(2), 0,
                tag_2, 1, HD(3), 0
        ),
        tag_1, HD(4),
        true,
        (
            2,  tag_1, 3, HD(1), HD(2), HD(4), 0,
                tag_2, 1, HD(3), 0
        )
    );
    TEST_LIST_INSERT(7,
        (
            2,  tag_1, 3, HD(1), HD(2), HD(4), 0,
                tag_2, 1, HD(3), 0
        ),
        tag_2, HD(5),
        true,
        (
            2,  tag_1, 3, HD(1), HD(2), HD(4), 0,
                tag_2, 2, HD(3), HD(5), 0
        )
    );
    TEST_LIST_INSERT(8,
        (
            2,  tag_1, 3, HD(1), HD(2), HD(4), 0,
                tag_2, 2, HD(3), HD(5), 0
        ),
        tag_2, HD(5),
        false,
        (
            2,  tag_1, 3, HD(1), HD(2), HD(4), 0,
                tag_2, 2, HD(3), HD(5), 0
        )
    );

    #undef TEST_LIST_INSERT

    TEST_CASE_OPEN(9)
    {
        TEST_REVERT_OPEN
        {
            clu_list_insert(NULL, &tag_1, HD(1));
        }
        TEST_REVERT_CLOSE
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(10)
    {
        list_p l = clu_list_create_immed(0);
        TEST_REVERT_OPEN
        {
            clu_list_insert(&l, NULL, HD(1));
        }
        TEST_REVERT_CLOSE
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(11)
    {
        list_p l = clu_list_create_immed(0);
        TEST_REVERT_OPEN
        {
            clu_list_insert(&l, &tag_1, NULL);
        }
        TEST_REVERT_CLOSE
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}

void test_list_remove(bool show)
{
    TEST_FN_OPEN

    tag_t tag_1 = clu_tag_format("test 1");
    tag_t tag_2 = clu_tag_format("test 2");
    tag_t tag_3 = clu_tag_format("test 3");
    tag_t tag_4 = clu_tag_format("test 4");

    #define TEST_LIST_REMOVE(TAG, L_BEF, HANDLER, RES, L_AFT)        \
    {                                                                       \
        TEST_CASE_OPEN(TAG)                                                 \
        {                                                                   \
            list_p l = clu_list_create_immed(ARG_OPEN L_BEF);   \
            bool res = clu_list_remove(&l, HANDLER);                  \
            assert(res == RES);                                             \
            assert(clu_list_immed(l, ARG_OPEN L_AFT));               \
        }                                                                   \
        TEST_CASE_CLOSE                                                     \
    }

    TEST_LIST_REMOVE(1,
        (
            4,  tag_1, 2, HD(1), HD(2), 0,
                tag_2, 2, HD(3), HD(4), 0,
                tag_3, 1, HD(5), 0,
                tag_4, 1, HD(6), 0
        ),
        HD(7),
        false,
        (
            4,  tag_1, 2, HD(1), HD(2), 0,
                tag_2, 2, HD(3), HD(4), 0,
                tag_3, 1, HD(5), 0,
                tag_4, 1, HD(6), 0
        )
    );
    TEST_LIST_REMOVE(2,
        (
            4,  tag_1, 2, HD(1), HD(2), 0,
                tag_2, 2, HD(3), HD(4), 0,
                tag_3, 1, HD(5), 0,
                tag_4, 1, HD(6), 0
        ),
        HD(5),
        true,
        (
            3,  tag_1, 2, HD(1), HD(2), 0,
                tag_2, 2, HD(3), HD(4), 0,
                tag_4, 1, HD(6), 0
        )
    );
    TEST_LIST_REMOVE(3,
        (
            3,  tag_1, 2, HD(1), HD(2), 0,
                tag_2, 2, HD(3), HD(4), 0,
                tag_4, 1, HD(6), 0
        ),
        HD(6),
        true,
        (
            2,  tag_1, 2, HD(1), HD(2), 0,
                tag_2, 2, HD(3), HD(4), 0
        )
    );
    TEST_LIST_REMOVE(4,
        (
            2,  tag_1, 2, HD(1), HD(2), 0,
                tag_2, 2, HD(3), HD(4), 0
        ),
        HD(4),
        true,
        (
            2,  tag_1, 2, HD(1), HD(2), 0,
                tag_2, 2, HD(3), HD(4), 1, HD(4)
        )
    );
    TEST_LIST_REMOVE(5,
        (
            2,  tag_1, 2, HD(1), HD(2), 0,
                tag_2, 2, HD(3), HD(4), 1, HD(4)
        ),
        HD(2),
        true,
        (
            2,  tag_1, 2, HD(1), HD(2), 1, HD(2),
                tag_2, 2, HD(3), HD(4), 1, HD(4)
        )
    );
    TEST_LIST_REMOVE(6,
        (
            2,  tag_1, 2, HD(1), HD(2), 1, HD(2),
                tag_2, 2, HD(3), HD(4), 1, HD(4)
        ),
        HD(1),
        true,
        (1,  tag_2, 2, HD(3), HD(4), 1, HD(4))
    );
    TEST_LIST_REMOVE(7,
        (1,  tag_2, 2, HD(3), HD(4), 1, HD(4)),
        HD(3),
        true,
        (0)
    );

    #undef TEST_LIST_REMOVE

    TEST_CASE_OPEN(8)
    {
        TEST_REVERT_OPEN
        {
            clu_list_remove(NULL, HD(1));
        }
        TEST_REVERT_CLOSE
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(9)
    {
        list_p l = clu_list_create_immed(0);
        TEST_REVERT_OPEN
        {
            clu_list_remove(&l, NULL);
        }
        TEST_REVERT_CLOSE
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}



void test_list_count(bool show)
{
    TEST_FN_OPEN

    tag_t tag_1 = clu_tag_format("test 1");
    tag_t tag_2 = clu_tag_format("test 1");

    #define TEST_LIST_COUNT(TAG, COUNT, ...)                       \
    {                                                                   \
        TEST_CASE_OPEN(TAG)                                             \
        {                                                               \
            list_p l = clu_list_create_immed(__VA_ARGS__);   \
            uint64_t count = clu_list_count(l);                   \
            assert(uint64(count, COUNT));                               \
            clu_list_free(l);                                     \
        }                                                               \
        TEST_CASE_CLOSE                                                 \
    }

    TEST_LIST_COUNT(1, 0,
        0
    );
    TEST_LIST_COUNT(2, 1,
        1,  tag_1, 1, HD(1), 0
    );
    TEST_LIST_COUNT(3, 2,
        2,  tag_1, 1, HD(1), 0,
            tag_2, 1, HD(2), 0
    );

    #undef TEST_LIST_COUNT

    TEST_FN_CLOSE
}

void test_list_get_body(bool show)
{
    TEST_FN_OPEN

    tag_t tag_1 = clu_tag_format("test 1");
    tag_t tag_2 = clu_tag_format("test 2");

    #define TEST_LIST_GET_BODY(TAG, INDEX, HANDLER, ...)           \
    {                                                                   \
        TEST_CASE_OPEN(TAG)                                             \
        {                                                               \
            list_p l = clu_list_create_immed(__VA_ARGS__);   \
            trie_p t = clu_list_get_trie(l, INDEX);               \
            if(HANDLER) {assert(clu_trie_contains(t, HANDLER));}        \
            else        {assert(t == NULL);}                            \
            clu_list_free(l);                                     \
        }                                                               \
        TEST_CASE_CLOSE                                                 \
    }

    TEST_LIST_GET_BODY(1, 0, NULL,
        0
    );
    TEST_LIST_GET_BODY(2, 1, NULL,
        0
    );
    TEST_LIST_GET_BODY(3, 0, HD(1),
        1,  tag_1, 1, HD(1), 0
    );
    TEST_LIST_GET_BODY(4, 1, NULL,
        1,  tag_1, 1, HD(1), 0
    );
    TEST_LIST_GET_BODY(5, 2, NULL,
        1,  tag_1, 1, HD(1), 0
    );
    TEST_LIST_GET_BODY(6, 0, HD(1),
        2,  tag_1, 1, HD(1), 0,
            tag_2, 1, HD(2), 0
    );
    TEST_LIST_GET_BODY(7, 1, HD(2),
        2,  tag_1, 1, HD(1), 0,
            tag_2, 1, HD(2), 0
    );
    TEST_LIST_GET_BODY(8, 2, NULL,
        2,  tag_1, 1, HD(1), 0,
            tag_2, 1, HD(2), 0
    );
    TEST_LIST_GET_BODY(9, 3, NULL,
        2,  tag_1, 1, HD(1), 0,
            tag_2, 1, HD(2), 0
    );

    #undef TEST_LIST_GET_BODY

    TEST_FN_CLOSE
}

void test_list_contains(bool show)
{
    TEST_FN_OPEN

    tag_t tag_1 = clu_tag_format("test 1");
    tag_t tag_2 = clu_tag_format("test 2");

    #define TEST_LIST_CONTAINS(TAG, HANDLER, RES, ...)             \
    {                                                                   \
        TEST_CASE_OPEN(TAG)                                             \
        {                                                               \
            list_p l = clu_list_create_immed(__VA_ARGS__);   \
            bool res = clu_list_contains(l, HANDLER);             \
            assert(res == RES);                                         \
            clu_list_free(l);                                     \
        }                                                               \
        TEST_CASE_CLOSE                                                 \
    }

    TEST_LIST_CONTAINS(1, HD(1), false,
        0
    );
    TEST_LIST_CONTAINS(2, HD(1), true,
        1,  tag_1, 1, HD(1), 0
    );
    TEST_LIST_CONTAINS(3, HD(2), false,
        1,  tag_1, 1, HD(1), 0
    );
    TEST_LIST_CONTAINS(4, HD(1), true,
        2,  tag_1, 1, HD(1), 0,
            tag_2, 1, HD(2), 0
    );
    TEST_LIST_CONTAINS(5, HD(2), true,
        2,  tag_1, 1, HD(1), 0,
            tag_2, 1, HD(2), 0
    );
    TEST_LIST_CONTAINS(5, HD(3), false,
        2,  tag_1, 1, HD(1), 0,
            tag_2, 1, HD(2), 0
    );

    #undef TEST_LIST_CONTAINS

    TEST_FN_CLOSE
}



void test_list()
{
    TEST_LIB

    bool show = false;

    test_list_create(show);
    test_list_pop(show);
    test_list_create_variadic(show);

    test_list_insert(show);
    test_list_remove(show);

    test_list_count(show);
    test_list_get_body(show);
    test_list_contains(show);

    TEST_ASSERT_MEM_EMPTY
}



int main()
{
    setbuf(stdout, NULL);
    test_list();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
