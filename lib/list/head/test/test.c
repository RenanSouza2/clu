#include <stdlib.h>

#include "../debug.h"
#include "../../body/debug.h"
#include "../../../mem/header.h"
#include "../../../tag/debug.h"
#include "../../../../utils/assert.h"



void test_list_head_create(bool show)
{
    printf("\n\t%s\t\t", __func__);

    tag_t tag = clu_tag_format("test");

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    list_head_p lh = clu_list_head_create(&tag, LB(1));
    assert(clu_tag(&lh->tag, &tag));
    assert(lh->lb == LB(1));
    assert(lh->lh == NULL);
    free(lh, list_head);

    assert(clu_mem_internal_empty());
}

void test_list_head_pop(bool show)
{
    printf("\n\t%s", __func__);

    tag_t tag = clu_tag_format("test");

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    list_head_p lh = clu_list_head_create(&tag, LB(1));
    lh = clu_list_head_pop(lh);
    assert(lh == NULL);

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    lh = clu_list_head_create(&tag, LB(1));
    lh->lh = clu_list_head_create(&tag, LB(2));
    lh = clu_list_head_pop(lh);
    assert(lh != NULL);
    assert(lh->lb == LB(2));
    free(lh, list_head);

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

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    lh = clu_list_head_create_immed(1,
        tag_1, 1, HD(1)
    );
    assert(lh != NULL);
    assert(lh->lb != NULL);
    assert(lh->lb->h == HD(1));
    assert(lh->lb->lb == NULL);
    assert(lh->lh == NULL);
    clu_list_head_free(&lh);

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    lh = clu_list_head_create_immed(1,
        tag_1, 2, HD(1), HD(2)
    );
    assert(lh != NULL);
    assert(lh->lb != NULL);
    assert(lh->lb->h == HD(1));
    assert(lh->lb->lb != NULL);
    assert(lh->lb->lb->h == HD(2));
    assert(lh->lb->lb->lb == NULL);
    assert(lh->lh == NULL);
    clu_list_head_free(&lh);

    if(show) printf("\n\t\t%s 4\t\t", __func__);
    lh = clu_list_head_create_immed(2,
        tag_1, 1, HD(1),
        tag_2, 1, HD(2)
    );
    assert(lh != NULL);
    assert(lh->lb != NULL);
    assert(lh->lb->h == HD(1));
    assert(lh->lb->lb == NULL);
    assert(lh->lh != NULL);
    assert(lh->lh->lb->h == HD(2));
    assert(lh->lh->lb->lb == NULL);
    assert(lh->lh->lh == NULL);
    clu_list_head_free(&lh);

    assert(clu_mem_internal_empty());
}



void test_list_head_create_handler(bool show)
{
    printf("\n\t%s\t\t", __func__);

    tag_t tag = clu_tag_format("test");

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    list_head_p lh = clu_list_head_create_handler(&tag, HD(1));
    assert(clu_list_head_immed(lh, 1,
        tag, 1, HD(1)
    ));

    assert(clu_mem_internal_empty());
}

void test_list_head_insert(bool show)
{
    printf("\n\t%s", __func__);

    tag_t tag_1 = clu_tag_format("test 1");
    tag_t tag_2 = clu_tag_format("test 2");

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    list_head_p lh = NULL;
    bool res = clu_list_head_insert(&lh, &tag_1, HD(1));
    assert(res == true);
    assert(clu_list_head_immed(lh, 1,
        tag_1, 1, HD(1)
    ));

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    lh = clu_list_head_create_immed(1,
        tag_1, 1, HD(1)
    );
    res = clu_list_head_insert(&lh, &tag_1, HD(1));
    assert(res == false);
    assert(clu_list_head_immed(lh, 1,
        tag_1, 1, HD(1)
    ));

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    lh = clu_list_head_create_immed(1,
        tag_1, 1, HD(1)
    );
    res = clu_list_head_insert(&lh, &tag_1, HD(2));
    assert(res == true);
    assert(clu_list_head_immed(lh, 1,
        tag_1, 2, HD(1), HD(2)
    ));

    if(show) printf("\n\t\t%s 4\t\t", __func__);
    lh = clu_list_head_create_immed(1,
        tag_1, 2, HD(1), HD(2)
    );
    printf("\na\t\t");
    res = clu_list_head_insert(&lh, &tag_2, HD(3));
    printf("\nb\t\t");
    assert(res == true);
    assert(clu_list_head_immed(lh, 2,
        tag_1, 2, HD(1), HD(2),
        tag_2, 1, HD(3)
    ));

    if(show) printf("\n\t\t%s 5\t\t", __func__);
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

    if(show) printf("\n\t\t%s 6\t\t", __func__);
    assert(clu_list_head_insert(&lh, &tag_2, HD(5)) == true);
    assert(clu_list_head_immed(lh, 2,
        tag_1, 3, HD(1), HD(2), HD(4),
        tag_2, 2, HD(3), HD(5)
    ));

    clu_list_head_free(&lh);
    assert(clu_mem_internal_empty());
}



// void test_list_head_remove()
// {
//     printf("\n\t%s", __func__);
//
//     tag_t tag1, tag2, tag3, tag4;
//     tag1 = clu_tag_format("test 1");
//     tag2 = clu_tag_format("test 2");
//     tag3 = clu_tag_format("test 3");
//     tag4 = clu_tag_format("test 4");
//
//     list_head_p lh = NULL;
//     clu_list_head_insert(&lh, HD(1), &tag1);
//     clu_list_head_insert(&lh, HD(2), &tag1);
//     clu_list_head_insert(&lh, HD(3), &tag2);
//     clu_list_head_insert(&lh, HD(4), &tag2);
//     clu_list_head_insert(&lh, HD(5), &tag3);
//     clu_list_head_insert(&lh, HD(6), &tag4);
//     assert(clu_list_head_immed(lh, 4,
//         tag1, 2, HD(1), HD(2),
//         tag2, 2, HD(3), HD(4),
//         tag3, 1, HD(5),
//         tag4, 1, HD(6)
//     ));
//
//     printf("\n\t\t%s 1", __func__);
//     assert(clu_list_head_remove(&lh, HD(7)) == false);
//     assert(clu_list_head_immed(lh, 4,
//         tag1, 2, HD(1), HD(2),
//         tag2, 2, HD(3), HD(4),
//         tag3, 1, HD(5),
//         tag4, 1, HD(6)
//     ));
//
//     printf("\n\t\t%s 2", __func__);
//     assert(clu_list_head_remove(&lh, HD(5)) == true);
//     assert(clu_list_head_immed(lh, 3,
//         tag1, 2, HD(1), HD(2),
//         tag2, 2, HD(3), HD(4),
//         tag4, 1, HD(6)
//     ));
//
//     printf("\n\t\t%s 3", __func__);
//     assert(clu_list_head_remove(&lh, HD(6)) == true);
//     assert(clu_list_head_immed(lh, 2,
//         tag1, 2, HD(1), HD(2),
//         tag2, 2, HD(3), HD(4)
//     ));
//
//     printf("\n\t\t%s 4", __func__);
//     assert(clu_list_head_remove(&lh, HD(4)) == true);
//     assert(clu_list_head_immed(lh, 2,
//         tag1, 2, HD(1), HD(2),
//         tag2, 1, HD(3)
//     ));
//
//     printf("\n\t\t%s 5", __func__);
//     assert(clu_list_head_remove(&lh, HD(2)) == true);
//     assert(clu_list_head_immed(lh, 2,
//         tag1, 1, HD(1),
//         tag2, 1, HD(3)
//     ));
//
//     printf("\n\t\t%s 6", __func__);
//     assert(clu_list_head_remove(&lh, HD(1)) == true);
//     assert(clu_list_head_immed(lh, 1,
//         tag2, 1, HD(3)
//     ));
//
//     printf("\n\t\t%s 6", __func__);
//     assert(clu_list_head_remove(&lh, HD(3)) == true);
//     assert(lh == NULL);
//
//     assert(clu_mem_internal_empty());
// }



void test_list_head()
{
    printf("\n%s", __func__);

    bool show = true;

    test_list_head_create(show);
    test_list_head_pop(show);
    test_list_head_create_variadic(show);

    test_list_head_create_handler(show);
    test_list_head_insert(show);
    // test_list_head_remove();

    assert(false);

    assert(clu_mem_internal_empty());
}



int main()
{
    setbuf(stdout, NULL);
    test_list_head();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
