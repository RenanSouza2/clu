#include <stdlib.h>

#include "../debug.h"
#include "../../body/debug.h"
#include "../../../mem/header.h"
#include "../../../tag/debug.h"
#include "../../../../utils/assert.h"
#include "../../../../utils/test_revert.h"



void test_list_head_create(bool show)
{
    printf("\n\t%s\t\t", __func__);

    tag_t tag = clu_tag_format("test");

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    list_head_p lh = clu_list_head_create(&tag);
    assert(clu_tag(&lh->tag, &tag));
    assert(lh->lb == NULL);
    assert(lh->lh == NULL);
    free(lh, list_head);

    assert(clu_mem_internal_empty());
}

void test_list_head_pop(bool show)
{
    printf("\n\t%s", __func__);

    tag_t tag = clu_tag_format("test");

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    list_head_p lh = clu_list_head_create(&tag);
    lh = clu_list_head_pop(lh);
    assert(lh == NULL);

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    lh = clu_list_head_create(&tag);
    lh->lb = LB(1);
    lh->lh = clu_list_head_create(&tag);
    lh->lh->lb = LB(2);
    lh = clu_list_head_pop(lh);
    assert(lh != NULL);
    assert(lh->lb == LB(2));
    free(lh, list_head);

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    TEST_REVERT_OPEN
    clu_list_head_pop(NULL);
    TEST_REVERT_CLOSE

    assert(clu_mem_internal_empty());
}

void test_list_head_create_variadic(bool show)
{
    printf("\n\t%s\t\t", __func__);

    tag_t tag_1 = clu_tag_format("test 1");
    tag_t tag_2 = clu_tag_format("test 2");

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    list_head_p lh = clu_list_head_create_immed(0);
    assert(lh == NULL);
    clu_list_head_free(lh);

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    lh = clu_list_head_create_immed(1,
        tag_1, 1, HD(1), 0
    );
    assert(lh != NULL);
    assert(clu_list_body_immed_list(lh->lb, 1, HD(1), 0))
    assert(lh->lh == NULL);
    clu_list_head_free(lh);

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    lh = clu_list_head_create_immed(1,
        tag_1, 2, HD(1), HD(2), 0
    );
    assert(lh != NULL);
    assert(clu_list_body_immed_list(lh->lb, 2, HD(1), HD(2), 0))
    assert(lh->lh == NULL);
    clu_list_head_free(lh);

    if(show) printf("\n\t\t%s 4\t\t", __func__);
    lh = clu_list_head_create_immed(2,
        tag_1, 1, HD(1), 0,
        tag_2, 1, HD(2), 0
    );
    assert(lh != NULL);
    assert(clu_list_body_immed_list(lh->lb, 1, HD(1), 0));
    assert(lh->lh != NULL);
    assert(clu_list_body_immed_list(lh->lh->lb, 1, HD(2), 0));
    assert(lh->lh->lh == NULL);
    clu_list_head_free(lh);

    assert(clu_mem_internal_empty());
}



void test_list_head_insert(bool show)
{
    printf("\n\t%s", __func__);

    tag_t tag_1 = clu_tag_format("test 1");
    tag_t tag_2 = clu_tag_format("test 2");

    #define TEST_LIST_HEAD_INSERT(TAG, LTAG, HANDLER, RES, ...)     \
    {                                                               \
        list_head_p lh[2];                                          \
        if(show) printf("\n\t\t%s %2d\t\t", __func__, TAG);         \
        clu_list_head_create_vec_immed(lh, 2, __VA_ARGS__);         \
        bool res = clu_list_head_insert(&lh[0], &LTAG, HANDLER);    \
        assert(res == RES);                                         \
        assert(clu_list_head_str(lh[0], lh[1]));                    \
    }

    TEST_LIST_HEAD_INSERT( 1, tag_1, HD(1), true,
        0,
        1,  tag_1, 1, HD(1), 0
    );
    TEST_LIST_HEAD_INSERT( 2, tag_1, HD(1), false,
        1,  tag_1, 1, HD(1), 0,
        1,  tag_1, 1, HD(1), 0
    );
    TEST_LIST_HEAD_INSERT( 3, tag_1, HD(2), true,
        1,  tag_1, 1, HD(1), 0,
        1,  tag_1, 2, HD(1), HD(2), 0
    );
    TEST_LIST_HEAD_INSERT( 4, tag_2, HD(3), true,
        1,  tag_1, 2, HD(1), HD(2), 0,
        2,  tag_1, 2, HD(1), HD(2), 0,
            tag_2, 1, HD(3), 0
    );
    TEST_LIST_HEAD_INSERT( 5, tag_1, HD(4), true,
        2,  tag_1, 2, HD(1), HD(2), 0,
            tag_2, 1, HD(3), 0,
        2,  tag_1, 3, HD(1), HD(2), HD(4), 0,
            tag_2, 1, HD(3), 0
    );
    TEST_LIST_HEAD_INSERT( 6, tag_2, HD(5), true,
        2,  tag_1, 3, HD(1), HD(2), HD(4), 0,
            tag_2, 1, HD(3), 0,
        2,  tag_1, 3, HD(1), HD(2), HD(4), 0,
            tag_2, 2, HD(3), HD(5), 0
    );
    TEST_LIST_HEAD_INSERT( 7, tag_2, HD(5), false,
        2,  tag_1, 3, HD(1), HD(2), HD(4), 0,
            tag_2, 2, HD(3), HD(5), 0,
        2,  tag_1, 3, HD(1), HD(2), HD(4), 0,
            tag_2, 2, HD(3), HD(5), 0
    );

    #undef TEST_LIST_HEAD_INSERT

    if(show) printf("\n\t\t%s  8\t\t", __func__);
    TEST_REVERT_OPEN
    clu_list_head_insert(NULL, &tag_1, HD(1));
    TEST_REVERT_CLOSE

    if(show) printf("\n\t\t%s  9\t\t", __func__);
    list_head_p lh = clu_list_head_create_immed(0);
    TEST_REVERT_OPEN
    clu_list_head_insert(&lh, NULL, HD(1));
    TEST_REVERT_CLOSE

    if(show) printf("\n\t\t%s 10\t\t", __func__);
    lh = clu_list_head_create_immed(0);
    TEST_REVERT_OPEN
    clu_list_head_insert(&lh, &tag_1, NULL);
    TEST_REVERT_CLOSE

    assert(clu_mem_internal_empty());
}

void test_list_head_remove(bool show)
{
    printf("\n\t%s", __func__);

    tag_t tag_1, tag_2, tag_3, tag_4;
    tag_1 = clu_tag_format("test 1");
    tag_2 = clu_tag_format("test 2");
    tag_3 = clu_tag_format("test 3");
    tag_4 = clu_tag_format("test 4");

    #define TEST_LIST_HEAD_REMOVE(TAG, HANDLER, RES, ...)   \
    {                                                       \
        list_head_p lh[2];                                  \
        if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);  \
        clu_list_head_create_vec_immed(lh, 2, __VA_ARGS__); \
        bool res = clu_list_head_remove(&lh[0], HANDLER);   \
        assert(res == RES);                                 \
        assert(clu_list_head_str(lh[0], lh[1]));            \
    }

    TEST_LIST_HEAD_REMOVE(1, HD(7), false,
        4,  tag_1, 2, HD(1), HD(2), 0,
            tag_2, 2, HD(3), HD(4), 0,
            tag_3, 1, HD(5), 0,
            tag_4, 1, HD(6), 0,
        4,  tag_1, 2, HD(1), HD(2), 0,
            tag_2, 2, HD(3), HD(4), 0,
            tag_3, 1, HD(5), 0,
            tag_4, 1, HD(6), 0
    );
    TEST_LIST_HEAD_REMOVE(2, HD(5), true,
        4,  tag_1, 2, HD(1), HD(2), 0,
            tag_2, 2, HD(3), HD(4), 0,
            tag_3, 1, HD(5), 0,
            tag_4, 1, HD(6), 0,
        3,  tag_1, 2, HD(1), HD(2), 0,
            tag_2, 2, HD(3), HD(4), 0,
            tag_4, 1, HD(6), 0
    );
    TEST_LIST_HEAD_REMOVE(3, HD(6), true,
        3,  tag_1, 2, HD(1), HD(2), 0,
            tag_2, 2, HD(3), HD(4), 0,
            tag_4, 1, HD(6), 0,
        2,  tag_1, 2, HD(1), HD(2), 0,
            tag_2, 2, HD(3), HD(4), 0
    );
    TEST_LIST_HEAD_REMOVE(4, HD(4), true,
        2,  tag_1, 2, HD(1), HD(2), 0,
            tag_2, 2, HD(3), HD(4), 0,
        2,  tag_1, 2, HD(1), HD(2), 0,
            tag_2, 2, HD(3), HD(4), 1, HD(4)
    );
    TEST_LIST_HEAD_REMOVE(5, HD(2), true,
        2,  tag_1, 2, HD(1), HD(2), 0,
            tag_2, 2, HD(3), HD(4), 1, HD(4),
        2,  tag_1, 2, HD(1), HD(2), 1, HD(2),
            tag_2, 2, HD(3), HD(4), 1, HD(4)
    );
    TEST_LIST_HEAD_REMOVE(6, HD(1), true,
        2,  tag_1, 2, HD(1), HD(2), 1, HD(2),
            tag_2, 2, HD(3), HD(4), 1, HD(4),
        1,  tag_2, 2, HD(3), HD(4), 1, HD(4)
    );
    TEST_LIST_HEAD_REMOVE(7, HD(3), true,
        1,  tag_2, 2, HD(3), HD(4), 1, HD(4),
        0
    );

    #undef TEST_LIST_HEAD_REMOVE

    if(show) printf("\n\t\t%s 8", __func__);
    TEST_REVERT_OPEN
    clu_list_head_remove(NULL, HD(1));
    TEST_REVERT_CLOSE

    if(show) printf("\n\t\t%s 9", __func__);
    list_head_p lh = clu_list_head_create_immed(0);
    TEST_REVERT_OPEN
    clu_list_head_remove(&lh, NULL);
    TEST_REVERT_CLOSE

    assert(clu_mem_internal_empty());
}



void test_list_head_count(bool show)
{
    printf("\n\t%s\t\t", __func__);

    tag_t tag_1 = clu_tag_format("test 1");
    tag_t tag_2 = clu_tag_format("test 1");

    #define TEST_LIST_HEAD_COUNT(TAG, COUNT, ...)                   \
    {                                                               \
        if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);          \
        list_head_p lh = clu_list_head_create_immed(__VA_ARGS__);   \
        uint64_t count = clu_list_head_count(lh);                   \
        assert(uint64(count, COUNT));                               \
        clu_list_head_free(lh);                                     \
    }

    TEST_LIST_HEAD_COUNT(1, 0,
        0
    );
    TEST_LIST_HEAD_COUNT(2, 1,
        1,  tag_1, 1, HD(1), 0
    );
    TEST_LIST_HEAD_COUNT(3, 2,
        2,  tag_1, 1, HD(1), 0,
            tag_2, 1, HD(2), 0
    );

    #undef TEST_LIST_HEAD_COUNT

    assert(clu_mem_internal_empty());
}

void test_list_head_get_body(bool show)
{
    printf("\n\t%s\t\t", __func__);

    tag_t tag_1 = clu_tag_format("test 1");
    tag_t tag_2 = clu_tag_format("test 2");

    #define TEST_LIST_HEAD_GET_BODY(TAG, INDEX, HANDLER, ...)       \
    {                                                               \
        if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);          \
        list_head_p lh = clu_list_head_create_immed(__VA_ARGS__);   \
        list_body_p lb = clu_list_head_get_body(lh, INDEX);         \
        if(HANDLER) {assert(clu_list_body_contains(lb, HANDLER));}  \
        else        {assert(lb == NULL);}                           \
        clu_list_head_free(lh);                                     \
    }

    TEST_LIST_HEAD_GET_BODY(1, 0, NULL,
        0
    );
    TEST_LIST_HEAD_GET_BODY(2, 1, NULL,
        0
    );
    TEST_LIST_HEAD_GET_BODY(3, 0, HD(1),
        1,  tag_1, 1, HD(1), 0
    );
    TEST_LIST_HEAD_GET_BODY(4, 1, NULL,
        1,  tag_1, 1, HD(1), 0
    );
    TEST_LIST_HEAD_GET_BODY(5, 2, NULL,
        1,  tag_1, 1, HD(1), 0
    );
    TEST_LIST_HEAD_GET_BODY(6, 0, HD(1),
        2,  tag_1, 1, HD(1), 0,
            tag_2, 1, HD(2), 0
    );
    TEST_LIST_HEAD_GET_BODY(7, 1, HD(2),
        2,  tag_1, 1, HD(1), 0,
            tag_2, 1, HD(2), 0
    );
    TEST_LIST_HEAD_GET_BODY(7, 2, NULL,
        2,  tag_1, 1, HD(1), 0,
            tag_2, 1, HD(2), 0
    );
    TEST_LIST_HEAD_GET_BODY(8, 3, NULL,
        2,  tag_1, 1, HD(1), 0,
            tag_2, 1, HD(2), 0
    );

    #undef TEST_LIST_HEAD_GET_BODY

    assert(clu_mem_internal_empty());
}

void test_list_head_contains(bool show)
{
    printf("\n\t%s\t\t", __func__);

    tag_t tag_1 = clu_tag_format("test 1");
    tag_t tag_2 = clu_tag_format("test 2");

    #define TEST_LIST_HEAD_CONTAINS(TAG, HANDLER, RES, ...)         \
    {                                                               \
        if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);          \
        list_head_p lh = clu_list_head_create_immed(__VA_ARGS__);   \
        bool res = clu_list_head_contains(lh, HANDLER);             \
        assert(res == RES);                                         \
        clu_list_head_free(lh);                                     \
    }

    TEST_LIST_HEAD_CONTAINS(1, HD(1), false,
        0
    );
    TEST_LIST_HEAD_CONTAINS(2, HD(1), true,
        1,  tag_1, 1, HD(1), 0
    );
    TEST_LIST_HEAD_CONTAINS(3, HD(2), false,
        1,  tag_1, 1, HD(1), 0
    );
    TEST_LIST_HEAD_CONTAINS(4, HD(1), true,
        2,  tag_1, 1, HD(1), 0,
            tag_2, 1, HD(2), 0
    );
    TEST_LIST_HEAD_CONTAINS(5, HD(2), true,
        2,  tag_1, 1, HD(1), 0,
            tag_2, 1, HD(2), 0
    );
    TEST_LIST_HEAD_CONTAINS(5, HD(3), false,
        2,  tag_1, 1, HD(1), 0,
            tag_2, 1, HD(2), 0
    );

    #undef TEST_LIST_HEAD_CONTAINS

    assert(clu_mem_internal_empty());
}



void test_list_head()
{
    printf("\n%s", __func__);

    bool show = false;

    test_list_head_create(show);
    test_list_head_pop(show);
    test_list_head_create_variadic(show);

    test_list_head_insert(show);
    test_list_head_remove(show);

    test_list_head_count(show);
    test_list_head_get_body(show);
    test_list_head_contains(show);

    assert(clu_mem_internal_empty());
}



int main()
{
    setbuf(stdout, NULL);
    test_list_head();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
