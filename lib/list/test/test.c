#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "../debug.h"



void free_body(list_body_p lb)
{
    while(lb) lb = list_body_pop(lb);
}

void free_head(list_head_p lh)
{
    while(lh)
    {
        free_body(lh->lb);
        lh = list_head_pop(lh);
    }
}



list_head_p list_head_create_test(char str_s[], list_head_p lh_next)
{
    string_p str;
    string_convert(str, str_s);
    return list_head_create(str, lh_next);
}

list_head_p list_head_find_test(list_head_p lh, char str_s[])
{
    string_p str;
    string_convert(str, str_s);
    return list_head_find(lh, str);
}



void string_convert(string_p str, char str_s[]);



void test_string_convert()
{
    printf("\n\ttest string convert\t\t");

    string_p str;
    string_convert(str, "test");
    assert(strcmp(str, "test") == 0);

    assert(list_memory());
}



void test_list_body_create()
{
    printf("\n\t\ttest list body create\t\t");
    
    list_body_p lb = list_body_create((handler_p)1, LB(2));
    assert(lb->h  == (handler_p)1);
    assert(lb->lb == LB(2));
    free(lb, list_body);

    assert(list_memory());
}

void test_list_body_pop()
{
    printf("\n\t\ttest list body pop\t\t");

    list_body_p lb = list_body_create(NULL, LB(1));
    lb = list_body_pop(lb);
    assert(lb == LB(1));

    assert(list_memory());
}

void test_list_body_remove()
{
    printf("\n\t\ttest list body remove\t\t");

    list_body_p lb = list_body_create((handler_p)1, NULL);
    lb = list_body_create(NULL, lb);
    assert(list_body_remove(lb, (handler_p)1));
    assert(lb->lb == NULL);

    assert(list_body_remove(lb, (handler_p)1) == false);
    free_body(lb);

    list_head_p lh = list_head_create_test("test", NULL);
    lh->lb = list_body_create((handler_p)1, NULL);
    assert(list_body_remove(LB(lh), (handler_p)1));
    assert(lh->lb == NULL);
    free_head(lh);

    assert(list_memory());
}

void test_list_body_operations()
{
    printf("\n\ttest list body operations\t\t");

    test_list_body_create();
    test_list_body_pop();
    test_list_body_remove();

    assert(list_memory());
}



void test_list_head_create()
{
    printf("\n\t\ttest list head create\t\t");

    list_head_p lh = list_head_create_test("test", LH(1));
    assert(lh->lh == LH(1));
    assert(lh->lb == NULL);
    assert(strcmp(lh->str, "test") == 0);
    free(lh, list_head);

    lh = list_head_create_test("", NULL);
    assert(strcmp(lh->str, "") == 0);
    free(lh, list_head);

    lh = list_head_create_test("abcdefghi abcdefghi abcdefghi abcdefghi abcdefghi", NULL);
    assert(strcmp(lh->str, "abcdefghi abcdefghi abcdefghi abcdefghi abcdefghi") == 0);
    free(lh, list_head);

    assert(list_memory());
}

void test_list_head_pop()
{
    printf("\n\t\ttest list head pop\t\t");

    list_head_p lh = list_head_create_test("test", LH(1));
    lh = list_head_pop(lh);
    assert(lh == LH(1));

    assert(list_memory());
}

void test_list_head_find()
{
    printf("\n\t\ttest list head find\t\t");

    list_head_p lh = NULL;

    list_head_p lh_res = list_head_find_test(lh, "test");
    assert(lh_res == NULL);

    lh = list_head_create_test("test", NULL);
    lh_res = list_head_find_test(lh, "test");
    assert(lh == lh_res);

    free(lh, list_head);
    assert(list_memory());
}

void test_list_head_operations()
{
    printf("\n\ttest list head operations\t\t");

    test_list_head_create();
    test_list_head_pop();
    test_list_head_find();

    assert(list_memory());
}



void test_insert()
{
    printf("\n\ttest insert\t\t");

    list_head_p lh = NULL;
    lh = list_insert(lh, (handler_p)1, "test 1");
    assert(lh);
    assert(strcmp(lh->str, "test 1") == 0);
    assert(lh->lh == NULL);
    assert(lh->lb);
    assert(lh->lb->h == (handler_p)1);

    lh = list_insert(lh, (handler_p)2, "test 1");
    assert(lh);
    assert(strcmp(lh->str, "test 1") == 0);
    assert(lh->lh == NULL);
    assert(lh->lb);
    assert(lh->lb->h == (handler_p)2);

    lh = list_insert(lh, (handler_p)3, "test 2");
    assert(strcmp(lh->str, "test 2") == 0);
    assert(lh->lb);
    assert(lh->lb->h == (handler_p)3);
    assert(lh->lh);
    assert(strcmp(lh->lh->str, "test 1") == 0);

    free_head(lh);
    assert(list_memory());
}

void test_remove()
{
    printf("\n\ttest remove\t\t");

    list_head_p lh = NULL;
    lh = list_insert(lh, (handler_p)1, "test 1");
    lh = list_insert(lh, (handler_p)2, "test 1");
    lh = list_insert(lh, (handler_p)3, "test 2");

    list_remove(LH(&lh), (handler_p)2);
    assert(lh->lh);
    assert(lh->lh->lb->h == (handler_p)1);

    list_remove(LH(&lh), (handler_p)1);
    assert(lh->lh == NULL);

    list_remove(LH(&lh), (handler_p)3);
    assert(lh == NULL);

    assert(list_memory());
}



void test_mem()
{
    printf("\ntest mem library\t\t");

    test_string_convert();
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
