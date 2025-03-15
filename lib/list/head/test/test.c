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

    if(show) printf("\n\t\t%s  1\t\t", __func__);
    list_head_p lh = clu_list_head_create_immed(0);
    bool res = clu_list_head_insert(&lh, &tag_1, HD(1));
    assert(res == true);
    assert(clu_list_head_immed(lh, 1,
        tag_1, 1, HD(1)
    ));

    if(show) printf("\n\t\t%s  2\t\t", __func__);
    lh = clu_list_head_create_immed(1,
        tag_1, 1, HD(1)
    );
    res = clu_list_head_insert(&lh, &tag_1, HD(1));
    assert(res == false);
    assert(clu_list_head_immed(lh, 1,
        tag_1, 1, HD(1)
    ));

    if(show) printf("\n\t\t%s  3\t\t", __func__);
    lh = clu_list_head_create_immed(1,
        tag_1, 1, HD(1)
    );
    res = clu_list_head_insert(&lh, &tag_1, HD(2));
    assert(res == true);
    assert(clu_list_head_immed(lh, 1,
        tag_1, 2, HD(1), HD(2)
    ));

    if(show) printf("\n\t\t%s  4\t\t", __func__);
    lh = clu_list_head_create_immed(1,
        tag_1, 2, HD(1), HD(2)
    );
    res = clu_list_head_insert(&lh, &tag_2, HD(3));
    assert(res == true);
    assert(clu_list_head_immed(lh, 2,
        tag_1, 2, HD(1), HD(2),
        tag_2, 1, HD(3)
    ));

    if(show) printf("\n\t\t%s  5\t\t", __func__);
    lh = clu_list_head_create_immed(2,
        tag_1, 2, HD(1), HD(2),
        tag_2, 1, HD(3)
    );
    res = clu_list_head_insert(&lh, &tag_1, HD(4));
    assert(res == true);
    assert(clu_list_head_immed(lh, 2,
        tag_1, 3, HD(1), HD(2), HD(4),
        tag_2, 1, HD(3)
    ));

    if(show) printf("\n\t\t%s  6\t\t", __func__);
    lh = clu_list_head_create_immed(2,
        tag_1, 3, HD(1), HD(2), HD(4),
        tag_2, 1, HD(3)
    );
    res = clu_list_head_insert(&lh, &tag_2, HD(5));
    assert(res == true);
    assert(clu_list_head_immed(lh, 2,
        tag_1, 3, HD(1), HD(2), HD(4),
        tag_2, 2, HD(3), HD(5)
    ));

    if(show) printf("\n\t\t%s  7\t\t", __func__);
    lh = clu_list_head_create_immed(2,
        tag_1, 3, HD(1), HD(2), HD(4),
        tag_2, 2, HD(3), HD(5)
    );
    res = clu_list_head_insert(&lh, &tag_2, HD(5));
    assert(res == false);
    assert(clu_list_head_immed(lh, 2,
        tag_1, 3, HD(1), HD(2), HD(4),
        tag_2, 2, HD(3), HD(5)
    ));

    if(show) printf("\n\t\t%s  8\t\t", __func__);
    TEST_REVERT_OPEN
    clu_list_head_insert(NULL, &tag_1, HD(1));
    TEST_REVERT_CLOSE

    if(show) printf("\n\t\t%s  9\t\t", __func__);
    lh = clu_list_head_create_immed(0);
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

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    list_head_p lh = clu_list_head_create_immed(4,
        tag_1, 2, HD(1), HD(2),
        tag_2, 2, HD(3), HD(4),
        tag_3, 1, HD(5),
        tag_4, 1, HD(6)
    );
    bool res = clu_list_head_remove(&lh, HD(7));
    assert(res == false);
    assert(clu_list_head_immed(lh, 4,
        tag_1, 2, HD(1), HD(2),
        tag_2, 2, HD(3), HD(4),
        tag_3, 1, HD(5),
        tag_4, 1, HD(6)
    ));

    if(show) printf("\n\t\t%s 2", __func__);
    lh = clu_list_head_create_immed(4,
        tag_1, 2, HD(1), HD(2),
        tag_2, 2, HD(3), HD(4),
        tag_3, 1, HD(5),
        tag_4, 1, HD(6)
    );
    res = clu_list_head_remove(&lh, HD(5));
    assert(res == true);
    assert(clu_list_head_immed(lh, 3,
        tag_1, 2, HD(1), HD(2),
        tag_2, 2, HD(3), HD(4),
        tag_4, 1, HD(6)
    ));

    if(show) printf("\n\t\t%s 3", __func__);
    lh = clu_list_head_create_immed(3,
        tag_1, 2, HD(1), HD(2),
        tag_2, 2, HD(3), HD(4),
        tag_4, 1, HD(6)
    );
    res = clu_list_head_remove(&lh, HD(6));
    assert(res == true);
    assert(clu_list_head_immed(lh, 2,
        tag_1, 2, HD(1), HD(2),
        tag_2, 2, HD(3), HD(4)
    ));

    if(show) printf("\n\t\t%s 4", __func__);
    lh = clu_list_head_create_immed(2,
        tag_1, 2, HD(1), HD(2),
        tag_2, 2, HD(3), HD(4)
    );
    res = clu_list_head_remove(&lh, HD(4));
    assert(res == true);
    assert(clu_list_head_immed(lh, 2,
        tag_1, 2, HD(1), HD(2),
        tag_2, 1, HD(3)
    ));

    if(show) printf("\n\t\t%s 5", __func__);
    lh = clu_list_head_create_immed(2,
        tag_1, 2, HD(1), HD(2),
        tag_2, 1, HD(3)
    );
    assert(clu_list_head_remove(&lh, HD(2)) == true);
    assert(clu_list_head_immed(lh, 2,
        tag_1, 1, HD(1),
        tag_2, 1, HD(3)
    ));

    if(show) printf("\n\t\t%s 6", __func__);
    lh = clu_list_head_create_immed(2,
        tag_1, 1, HD(1),
        tag_2, 1, HD(3)
    );
    res = clu_list_head_remove(&lh, HD(1));
    assert(res == true);
    assert(clu_list_head_immed(lh, 1,
        tag_2, 1, HD(3)
    ));

    if(show) printf("\n\t\t%s 7", __func__);
    lh = clu_list_head_create_immed(1,
        tag_2, 1, HD(3)
    );
    res = clu_list_head_remove(&lh, HD(3));
    assert(res == true);
    assert(lh == NULL);
    
    if(show) printf("\n\t\t%s 8", __func__);
    TEST_REVERT_OPEN
    clu_list_head_remove(NULL, HD(1));
    TEST_REVERT_CLOSE
    
    if(show) printf("\n\t\t%s 9", __func__);
    lh = clu_list_head_create_immed(0);
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

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    list_head_p lh = clu_list_head_create_immed(0);
    uint64_t count = clu_list_head_count(lh);
    assert(uint64(count, 0));

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    lh = clu_list_head_create_immed(1,
        tag_1, 1, HD(1)
    );
    count = clu_list_head_count(lh);
    assert(uint64(count, 1));
    clu_list_head_free(lh);

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    lh = clu_list_head_create_immed(2,
        tag_1, 1, HD(1),
        tag_2, 1, HD(2)
    );
    count = clu_list_head_count(lh);
    assert(uint64(count, 2));
    clu_list_head_free(lh);

    assert(clu_mem_internal_empty());
}

void test_list_head_get_body(bool show)
{
    printf("\n\t%s\t\t", __func__);

    tag_t tag_1 = clu_tag_format("test 1");
    tag_t tag_2 = clu_tag_format("test 2");

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    list_head_p lh = clu_list_head_create_immed(0);
    list_body_p lb = clu_list_head_get_body(lh, 0);
    assert(lb == NULL);

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    lh = clu_list_head_create_immed(0);
    lb = clu_list_head_get_body(lh, 1);
    assert(lb == NULL);

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    lh = clu_list_head_create_immed(1,
        tag_1, 1, HD(1)
    );
    lb = clu_list_head_get_body(lh, 0);
    assert(lb != NULL);
    assert(clu_list_body_contains(lb, HD(1)));
    clu_list_head_free(lh);

    if(show) printf("\n\t\t%s 4\t\t", __func__);
    lh = clu_list_head_create_immed(1,
        tag_1, 1, HD(1)
    );
    lb = clu_list_head_get_body(lh, 1);
    assert(lb == NULL);
    clu_list_head_free(lh);

    if(show) printf("\n\t\t%s 5\t\t", __func__);
    lh = clu_list_head_create_immed(1,
        tag_1, 1, HD(1)
    );
    lb = clu_list_head_get_body(lh, 2);
    assert(lb == NULL);
    clu_list_head_free(lh);

    if(show) printf("\n\t\t%s 6\t\t", __func__);
    lh = clu_list_head_create_immed(2,
        tag_1, 1, HD(1),
        tag_2, 1, HD(2)
    );
    lb = clu_list_head_get_body(lh, 0);
    assert(lb != NULL);
    assert(clu_list_body_contains(lb, HD(1)));
    clu_list_head_free(lh);

    if(show) printf("\n\t\t%s 7\t\t", __func__);
    lh = clu_list_head_create_immed(2,
        tag_1, 1, HD(1),
        tag_2, 1, HD(2)
    );
    lb = clu_list_head_get_body(lh, 1);
    assert(lb != NULL);
    assert(clu_list_body_contains(lb, HD(2)));
    clu_list_head_free(lh);

    if(show) printf("\n\t\t%s 8\t\t", __func__);
    lh = clu_list_head_create_immed(2,
        tag_1, 1, HD(1),
        tag_2, 1, HD(2)
    );
    lb = clu_list_head_get_body(lh, 3);
    assert(lb == NULL);
    clu_list_head_free(lh);

    assert(clu_mem_internal_empty());
}

void test_list_head_contains(bool show)
{
    printf("\n\t%s\t\t", __func__);

    tag_t tag_1 = clu_tag_format("test 1");
    tag_t tag_2 = clu_tag_format("test 2");

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    list_head_p lh = clu_list_head_create_immed(0);
    bool res = clu_list_head_contains(lh, HD(1));
    assert(res == false);

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    lh = clu_list_head_create_immed(1,
        tag_1, 1, HD(1)
    );
    res = clu_list_head_contains(lh, HD(1));
    assert(res == true);
    clu_list_head_free(lh);

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    lh = clu_list_head_create_immed(1,
        tag_1, 1, HD(1)
    );
    res = clu_list_head_contains(lh, HD(2));
    assert(res == false);
    clu_list_head_free(lh);

    if(show) printf("\n\t\t%s 4\t\t", __func__);
    lh = clu_list_head_create_immed(2,
        tag_1, 1, HD(1),
        tag_2, 1, HD(2)
    );
    res = clu_list_head_contains(lh, HD(1));
    assert(res == true);
    clu_list_head_free(lh);

    if(show) printf("\n\t\t%s 5\t\t", __func__);
    lh = clu_list_head_create_immed(2,
        tag_1, 1, HD(1),
        tag_2, 1, HD(2)
    );
    res = clu_list_head_contains(lh, HD(2));
    assert(res == true);
    clu_list_head_free(lh);

    if(show) printf("\n\t\t%s 6\t\t", __func__);
    lh = clu_list_head_create_immed(2,
        tag_1, 1, HD(1),
        tag_2, 1, HD(2)
    );
    res = clu_list_head_contains(lh, HD(3));
    assert(res == false);
    clu_list_head_free(lh);

    assert(clu_mem_internal_empty());
}



void test_list_head()
{
    printf("\n%s", __func__);

    bool show = true;

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
