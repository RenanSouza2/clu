#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "../debug.h"



void test_tag_convert()
{
    printf("\n\t\t%s\t\t", __func__);

    tag_t tag = tag_convert("test");
    assert(strcmp(tag.str, "test") == 0);
    
    tag = tag_convert("0123456789012345678901234567890123456789012345678");
    assert(strcmp(tag.str, "0123456789012345678901234567890123456789012345678") == 0);

    assert(list_memory());
}

void test_tag_eq()
{
    printf("\n\t\t%s\t\t", __func__);

    tag_t tag1, tag2;
    tag1 = tag_convert("test");
    tag2 = tag_convert("teste");
    assert(tag_eq(&tag1, &tag2) == false);
    
    tag2 = tag_convert("test");
    assert(tag_eq(&tag1, &tag2) == true);
    
    tag1 = tag_convert("0123456789012345678901234567890123456789012345678");
    tag2 = tag_convert("0123456789012345678901234567890123456789012345678");
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
    
    list_body_p lb = mem_list_body_create((handler_p)1, LB(2));
    assert(lb->h  == (handler_p)1);
    assert(lb->lb == LB(2));
    free(lb, list_body);

    assert(list_memory());
}

void test_list_body_pop()
{
    printf("\n\t\t%s\t\t", __func__);

    list_body_p lb = mem_list_body_create(NULL, LB(1));
    lb = mem_list_body_pop(lb);
    assert(lb == LB(1));

    assert(list_memory());
}

void test_list_body_remove()
{
    printf("\n\t\t%s\t\t", __func__);

    list_body_p lb = mem_list_body_create(HD(1), NULL);
    
    assert(mem_list_body_remove(&lb, HD(2)) == false);
    assert(mem_list_body(lb, 1, HD(1)));

    assert(mem_list_body_remove(&lb, (handler_p)1) == true);
    assert(lb == NULL);

    assert(mem_list_body_remove(&lb, (handler_p)1) == false);
    mem_list_body_free(lb);

    assert(list_memory());
}

void test_list_body_operations()
{
    printf("\n\t%s\t\t", __func__);

    test_list_body_create();
    test_list_body_pop();
    test_list_body_remove();

    assert(list_memory());
}



void test_list_head_create()
{
    printf("\n\t\t%s\t\t", __func__);

    printf("\n\t\t\t%s 1\t\t", __func__);
    tag_t tag;
    tag = tag_convert("test");
    list_head_p lh = mem_list_head_create(&tag, HD(1));
    assert(mem_list_head(lh, 1,
        tag, 1, HD(1)
    ));
    mem_list_head_free(lh);

    printf("\n\t\t\t%s 2\t\t", __func__);
    tag = tag_convert("");
    lh = mem_list_head_create(&tag, HD(1));
    assert(mem_list_head(lh, 1,
        tag, 1, HD(1)
    ));
    mem_list_head_free(lh);

    printf("\n\t\t\t%s 3\t\t", __func__);
    tag = tag_convert("abcdefghi abcdefghi abcdefghi abcdefghi abcdefghi");
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
    tag = tag_convert("test");

    list_head_p lh = mem_list_head_create(&tag, HD(1));
    free(lh->lb, list_body);

    lh->lh = LH(1);
    lh = mem_list_head_pop(lh);
    assert(lh == LH(1));

    assert(list_memory());
}

void test_list_head_operations()
{
    printf("\n\t%s\t\t", __func__);

    test_list_head_create();
    test_list_head_pop();

    assert(list_memory());
}



void test_insert()
{
    printf("\n\t%s\t\t", __func__);

    printf("\n\t\t%s 1\t\t", __func__);
    list_head_p lh = NULL;
    mem_list_head_insert(&lh, (handler_p)1, "test 1");
    assert(lh);
    assert(strcmp(lh->tag.str, "test 1") == 0);
    assert(lh->lh == NULL);
    assert(lh->lb);
    assert(lh->lb->h == (handler_p)1);

    printf("\n\t\t%s 2\t\t", __func__);
    mem_list_head_insert(&lh, (handler_p)2, "test 1");
    assert(lh);
    assert(strcmp(lh->tag.str, "test 1") == 0);
    assert(lh->lh == NULL);
    assert(lh->lb);
    assert(lh->lb->h == (handler_p)2);

    printf("\n\t\t%s 3\t\t", __func__);
    mem_list_head_insert(&lh, (handler_p)3, "test 2");
    assert(strcmp(lh->tag.str, "test 2") == 0);
    assert(lh->lb);
    assert(lh->lb->h == (handler_p)3);
    assert(lh->lh);
    assert(strcmp(lh->lh->tag.str, "test 1") == 0);
    
    printf("\n\t\t%s 4\t\t", __func__);
    mem_list_head_insert(&lh, (handler_p)4, "test 1");
    assert(lh);
    assert(strcmp(lh->tag.str, "test 2") == 0);
    assert(lh->lh);
    assert(strcmp(lh->lh->tag.str, "test 1") == 0);
    assert(lh->lh->lb);
    assert(lh->lh->lb->h == (handler_p)4);

    mem_list_head_free(lh);
    assert(list_memory());
}

void test_remove()
{
    printf("\n\ttest remove\t\t");

    list_head_p lh = NULL;
    mem_list_head_insert(&lh, (handler_p)1, "test 1");
    mem_list_head_insert(&lh, (handler_p)2, "test 1");
    mem_list_head_insert(&lh, (handler_p)3, "test 2");

    mem_list_head_remove(&lh, (handler_p)4);
    assert(lh->lh);
    assert(lh->lh->lb->h == (handler_p)1);

    mem_list_head_remove(&lh, (handler_p)2);
    assert(lh->lh);
    assert(lh->lh->lb->h == (handler_p)1);

    mem_list_head_remove(&lh, (handler_p)1);
    assert(lh->lh == NULL);

    mem_list_head_remove(&lh, (handler_p)3);
    assert(lh == NULL);

    assert(list_memory());
}



void test_mem()
{
    printf("\ntest mem library\t\t");

    test_tag();
    test_list_body_operations();
    test_list_head_operations();
    test_insert();
    test_remove();

    assert(list_memory());
}



int main() 
{
    setbuf(stdout, NULL);
    test_mem();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
