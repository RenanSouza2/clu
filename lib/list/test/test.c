#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "../debug.h"



void test_tag_convert()
{
    printf("\n\t\t%s\t\t", __func__);

    printf("\n\t\t\t%s 1\t\t", __func__);
    tag_t tag = tag_convert_test("test");
    assert(strcmp(tag.str, "test") == 0);
    
    printf("\n\t\t\t%s 2\t\t", __func__);
    tag = tag_convert_test("0123456789012345678901234567890123456789012345678");
    assert(strcmp(tag.str, "0123456789012345678901234567890123456789012345678") == 0);

    assert(list_memory());
}

void test_tag_eq()
{
    printf("\n\t\t%s\t\t", __func__);

    tag_t tag1, tag2;
    tag1 = tag_convert_test("test");
    tag2 = tag_convert_test("teste");
    assert(tag_eq(&tag1, &tag2) == false);
    
    tag2 = tag_convert_test("test");
    assert(tag_eq(&tag1, &tag2) == true);
    
    tag1 = tag_convert_test("0123456789012345678901234567890123456789012345678");
    tag2 = tag_convert_test("0123456789012345678901234567890123456789012345678");
    assert(tag_eq(&tag1, &tag2) == true);

    assert(list_memory());
}

void test_tag()
{
    printf("\n\t%s\t\t", __func__);

    test_tag_convert();
    test_tag_eq();
    
    assert(list_memory());
}


void test_list_body_create()
{
    printf("\n\t\t%s\t\t", __func__);
    
    list_body_p lb = mem_list_body_create(HD(1));
    assert(lb->h  == HD(1));
    assert(lb->lb == NULL);
    free(lb, list_body);

    assert(list_memory());
}

void test_list_body_pop()
{
    printf("\n\t\t%s\t\t", __func__);

    list_body_p lb = mem_list_body_create(HD(1));
    lb->lb = LB(1);
    lb = mem_list_body_pop(lb);
    assert(lb == LB(1));

    assert(list_memory());
}

void test_list_body_insert()
{
    printf("\n\t\t%s\t\t", __func__);

    list_body_p lb = NULL;
    assert(mem_list_body_insert(&lb, HD(1)));
    assert(mem_list_body(lb, 1, HD(1)));

    assert(mem_list_body_insert(&lb, HD(1)) == false);
    
    assert(mem_list_body_insert(&lb, HD(2)));
    assert(mem_list_body(lb, 2, HD(1), HD(2)));
    mem_list_body_free(lb);

    assert(list_memory());
}

void test_list_body_remove()
{
    printf("\n\t\t%s\t\t", __func__);

    list_body_p lb = mem_list_body_create(HD(1));
    
    assert(mem_list_body_remove(&lb, HD(2)) == false);
    assert(mem_list_body(lb, 1, HD(1)));

    assert(mem_list_body_remove(&lb, (handler_p)1) == true);
    assert(lb == NULL);

    assert(mem_list_body_remove(&lb, (handler_p)1) == false);

    assert(list_memory());
}

void test_list_body_operations()
{
    printf("\n\t%s\t\t", __func__);

    test_list_body_create();
    test_list_body_pop();

    test_list_body_insert();
    test_list_body_remove();

    assert(list_memory());
}



void test_list_head_create()
{
    printf("\n\t\t%s\t\t", __func__);

    printf("\n\t\t\t%s 1\t\t", __func__);
    tag_t tag;
    tag = tag_convert_test("test");
    list_head_p lh = mem_list_head_create(&tag, HD(1));
    assert(mem_list_head(lh, 1,
        tag, 1, HD(1)
    ));
    mem_list_head_free(lh);

    printf("\n\t\t\t%s 2\t\t", __func__);
    tag = tag_convert_test("");
    lh = mem_list_head_create(&tag, HD(1));
    assert(mem_list_head(lh, 1,
        tag, 1, HD(1)
    ));
    mem_list_head_free(lh);

    printf("\n\t\t\t%s 3\t\t", __func__);
    tag = tag_convert_test("abcdefghi abcdefghi abcdefghi abcdefghi abcdefghi");
    lh = mem_list_head_create(&tag, HD(1));
    assert(mem_list_head(lh, 1,
        tag, 1, HD(1)
    ));
    mem_list_head_free(lh);

    assert(list_memory());
}

void test_list_head_pop()
{
    printf("\n\t\t%s\t\t", __func__);

    tag_t tag;
    tag = tag_convert_test("test");

    list_head_p lh = mem_list_head_create(&tag, HD(1));
    free(lh->lb, list_body);

    lh->lh = LH(1);
    lh = mem_list_head_pop(lh);
    assert(lh == LH(1));

    assert(list_memory());
}

void test_list_head_insert()
{
    printf("\n\t\t%s\t\t", __func__);

    printf("\n\t\t\t%s 1\t\t", __func__);
    list_head_p lh = NULL;
    tag_t tag1 = tag_convert_test("test 1");
    assert(mem_list_head_insert_test(&lh, HD(1), "test 1") == true);
    assert(mem_list_head(lh, 1,
        tag1, 1, HD(1)
    ));

    printf("\n\t\t\t%s 2\t\t", __func__);
    assert(mem_list_head_insert_test(&lh, HD(1), "test 1") == false);
    assert(mem_list_head(lh, 1,
        tag1, 1, HD(1)
    ));
    

    printf("\n\t\t\t%s 3\t\t", __func__);
    assert(mem_list_head_insert_test(&lh, HD(2), "test 1") == true);
    assert(mem_list_head(lh, 1,
        tag1, 2, HD(1), HD(2)
    ));

    printf("\n\t\t\t%s 4\t\t", __func__);
    tag_t tag2 = tag_convert_test("test 2", NULL);
    assert(mem_list_head_insert_test(&lh, HD(3), "test 2") == true);
    assert(mem_list_head(lh, 2,
        tag1, 2, HD(1), HD(2),
        tag2, 1, HD(3)
    ));
    
    printf("\n\t\t\t%s 5\t\t", __func__);
    assert(mem_list_head_insert_test(&lh, HD(4), "test 1") == true);
    assert(mem_list_head(lh, 2,
        tag1, 3, HD(1), HD(2), HD(4),
        tag2, 1, HD(3)
    ));
    
    printf("\n\t\t\t%s 5\t\t", __func__);
    assert(mem_list_head_insert_test(&lh, HD(5), "test 2") == true);
    assert(mem_list_head(lh, 2,
        tag1, 3, HD(1), HD(2), HD(4),
        tag2, 2, HD(3), HD(5)
    ));

    mem_list_head_free(lh);
    assert(list_memory());
}

void test_list_head_remove()
{
    printf("\n\t\t%s\t\t", __func__);

    tag_t tag1, tag2, tag3, tag4;
    tag1 = tag_convert_test("test 1");
    tag2 = tag_convert_test("test 2");
    tag3 = tag_convert_test("test 3");
    tag4 = tag_convert_test("test 4");

    list_head_p lh = NULL;
    mem_list_head_insert_test(&lh, HD(1), "test 1");
    mem_list_head_insert_test(&lh, HD(2), "test 1");
    mem_list_head_insert_test(&lh, HD(3), "test 2");
    mem_list_head_insert_test(&lh, HD(4), "test 2");
    mem_list_head_insert_test(&lh, HD(5), "test 3");
    mem_list_head_insert_test(&lh, HD(6), "test 4");
    assert(mem_list_head(lh, 4,
        tag1, 2, HD(1), HD(2),
        tag2, 2, HD(3), HD(4),
        tag3, 1, HD(5),
        tag4, 1, HD(6)
    ));

    printf("\n\t\t\t%s 1\t\t", __func__);
    assert(mem_list_head_remove(&lh, HD(7)) == false);
    assert(mem_list_head(lh, 4,
        tag1, 2, HD(1), HD(2),
        tag2, 2, HD(3), HD(4),
        tag3, 1, HD(5),
        tag4, 1, HD(6)
    ));

    printf("\n\t\t\t%s 2\t\t", __func__);
    assert(mem_list_head_remove(&lh, HD(5)) == true);
    assert(mem_list_head(lh, 3,
        tag1, 2, HD(1), HD(2),
        tag2, 2, HD(3), HD(4),
        tag4, 1, HD(6)
    ));

    printf("\n\t\t\t%s 3\t\t", __func__);
    assert(mem_list_head_remove(&lh, HD(6)) == true);
    assert(mem_list_head(lh, 2,
        tag1, 2, HD(1), HD(2),
        tag2, 2, HD(3), HD(4)
    ));

    printf("\n\t\t\t%s 4\t\t", __func__);
    assert(mem_list_head_remove(&lh, HD(4)) == true);
    assert(mem_list_head(lh, 2,
        tag1, 2, HD(1), HD(2),
        tag2, 1, HD(3)
    ));

    printf("\n\t\t\t%s 5\t\t", __func__);
    assert(mem_list_head_remove(&lh, HD(2)) == true);
    assert(mem_list_head(lh, 2,
        tag1, 1, HD(1),
        tag2, 1, HD(3)
    ));

    printf("\n\t\t\t%s 6\t\t", __func__);
    assert(mem_list_head_remove(&lh, HD(1)) == true);
    assert(mem_list_head(lh, 1,
        tag2, 1, HD(3)
    ));

    printf("\n\t\t\t%s 6\t\t", __func__);
    assert(mem_list_head_remove(&lh, HD(3)) == true);
    assert(lh == NULL);

    assert(list_memory());
}

void test_list_head_operations()
{
    printf("\n\t%s\t\t", __func__);

    test_list_head_create();
    test_list_head_pop();

    test_list_head_insert();
    test_list_head_remove();

    assert(list_memory());
}





void test_mem()
{
    printf("\ntest mem library\t\t");

    test_tag();
    test_list_body_operations();
    test_list_head_operations();

    assert(list_memory());
}



int main() 
{
    setbuf(stdout, NULL);
    test_mem();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
