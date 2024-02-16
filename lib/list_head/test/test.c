#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../debug.h"
#include "../../mem/header.h"
#include "../../../utils/assert.h"

#include "../../tag/debug.h"



void test_list_head_create()
{
    printf("\n\t%s", __func__);

    printf("\n\t\t%s 1", __func__);
    tag_t tag;
    tag = clu_tag_format("test");
    list_head_p lh = clu_list_head_create(&tag, HD(1));
    assert(clu_list_head(lh, 1,
        tag, 1, HD(1)
    ));
    clu_list_head_free(&lh);

    printf("\n\t\t%s 2", __func__);
    tag = clu_tag_format("");
    lh = clu_list_head_create(&tag, HD(1));
    assert(clu_list_head(lh, 1,
        tag, 1, HD(1)
    ));
    clu_list_head_free(&lh);

    printf("\n\t\t%s 3", __func__);
    tag = clu_tag_format("abcdefghi abcdefghi abcdefghi abcdefghi abcdefghi");
    lh = clu_list_head_create(&tag, HD(1));
    assert(clu_list_head(lh, 1,
        tag, 1, HD(1)
    ));
    clu_list_head_free(&lh);

    assert(clu_mem_empty());
}

void test_list_head_pop()
{
    printf("\n\t%s", __func__);

    tag_t tag;
    tag = clu_tag_format("test");

    list_head_p lh = clu_list_head_create(&tag, HD(1));
    free(lh->lb, list_body);

    lh->lh = LH(1);
    lh = clu_list_head_pop(lh);
    assert(lh == LH(1));

    assert(clu_mem_empty());
}

void test_list_head_insert()
{
    printf("\n\t%s", __func__);

    printf("\n\t\t%s 1", __func__);
    list_head_p lh = NULL;
    tag_t tag1 = clu_tag_format("test 1");
    assert(clu_list_head_insert(&lh, HD(1), &tag1) == true);
    assert(clu_list_head(lh, 1,
        tag1, 1, HD(1)
    ));

    printf("\n\t\t%s 2", __func__);
    assert(clu_list_head_insert(&lh, HD(1), &tag1) == false);
    assert(clu_list_head(lh, 1,
        tag1, 1, HD(1)
    ));
    
    printf("\n\t\t%s 3", __func__);
    assert(clu_list_head_insert(&lh, HD(2), &tag1) == true);
    assert(clu_list_head(lh, 1,
        tag1, 2, HD(1), HD(2)
    ));

    printf("\n\t\t%s 4", __func__);
    tag_t tag2 = clu_tag_format("test 2");
    assert(clu_list_head_insert(&lh, HD(3), &tag2) == true);
    assert(clu_list_head(lh, 2,
        tag1, 2, HD(1), HD(2),
        tag2, 1, HD(3)
    ));
    
    printf("\n\t\t%s 5", __func__);
    assert(clu_list_head_insert(&lh, HD(4), &tag1) == true);
    assert(clu_list_head(lh, 2,
        tag1, 3, HD(1), HD(2), HD(4),
        tag2, 1, HD(3)
    ));
    
    printf("\n\t\t%s 5", __func__);
    assert(clu_list_head_insert(&lh, HD(5), &tag2) == true);
    assert(clu_list_head(lh, 2,
        tag1, 3, HD(1), HD(2), HD(4),
        tag2, 2, HD(3), HD(5)
    ));

    clu_list_head_free(&lh);
    assert(clu_mem_empty());
}

void test_list_head_remove()
{
    printf("\n\t%s", __func__);

    tag_t tag1, tag2, tag3, tag4;
    tag1 = clu_tag_format("test 1");
    tag2 = clu_tag_format("test 2");
    tag3 = clu_tag_format("test 3");
    tag4 = clu_tag_format("test 4");

    list_head_p lh = NULL;
    clu_list_head_insert(&lh, HD(1), &tag1);
    clu_list_head_insert(&lh, HD(2), &tag1);
    clu_list_head_insert(&lh, HD(3), &tag2);
    clu_list_head_insert(&lh, HD(4), &tag2);
    clu_list_head_insert(&lh, HD(5), &tag3);
    clu_list_head_insert(&lh, HD(6), &tag4);
    assert(clu_list_head(lh, 4,
        tag1, 2, HD(1), HD(2),
        tag2, 2, HD(3), HD(4),
        tag3, 1, HD(5),
        tag4, 1, HD(6)
    ));

    printf("\n\t\t%s 1", __func__);
    assert(clu_list_head_remove(&lh, HD(7)) == false);
    assert(clu_list_head(lh, 4,
        tag1, 2, HD(1), HD(2),
        tag2, 2, HD(3), HD(4),
        tag3, 1, HD(5),
        tag4, 1, HD(6)
    ));

    printf("\n\t\t%s 2", __func__);
    assert(clu_list_head_remove(&lh, HD(5)) == true);
    assert(clu_list_head(lh, 3,
        tag1, 2, HD(1), HD(2),
        tag2, 2, HD(3), HD(4),
        tag4, 1, HD(6)
    ));

    printf("\n\t\t%s 3", __func__);
    assert(clu_list_head_remove(&lh, HD(6)) == true);
    assert(clu_list_head(lh, 2,
        tag1, 2, HD(1), HD(2),
        tag2, 2, HD(3), HD(4)
    ));

    printf("\n\t\t%s 4", __func__);
    assert(clu_list_head_remove(&lh, HD(4)) == true);
    assert(clu_list_head(lh, 2,
        tag1, 2, HD(1), HD(2),
        tag2, 1, HD(3)
    ));

    printf("\n\t\t%s 5", __func__);
    assert(clu_list_head_remove(&lh, HD(2)) == true);
    assert(clu_list_head(lh, 2,
        tag1, 1, HD(1),
        tag2, 1, HD(3)
    ));

    printf("\n\t\t%s 6", __func__);
    assert(clu_list_head_remove(&lh, HD(1)) == true);
    assert(clu_list_head(lh, 1,
        tag2, 1, HD(3)
    ));

    printf("\n\t\t%s 6", __func__);
    assert(clu_list_head_remove(&lh, HD(3)) == true);
    assert(lh == NULL);

    assert(clu_mem_empty());
}



void test_list_head()
{
    printf("\n%s", __func__);

    test_list_head_create();
    test_list_head_pop();

    test_list_head_insert();
    test_list_head_remove();

    assert(clu_mem_empty());
}



int main() 
{
    setbuf(stdout, NULL);
    test_list_head();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
