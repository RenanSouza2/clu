#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "debug.h"

#ifdef DEBUG

int list_head_alive;
int list_body_alive;

bool list_memory()
{
    return list_head_alive == 0 && list_body_alive == 0;
}

bool mem_list_body_variadic(list_body_p lb, va_list *args)
{
    int count_body = va_arg(*args, int);

    int i=0;
    for(; lb && (i<count_body); lb = lb->lb, i++)
    {
        handler_p h = va_arg(*args, handler_p);
        if(lb->h != h)
        {
            printf("\nMEM LIST BODY | ERROR 1 HANDLER MISMATCH | %d %d", i, count_body);
            return false;
        }
    }

    if(i<count_body)
    {
        printf("\nMEM LIST BODY | ERROR 2 LIST SHORTER | %d %d", i, count_body);
        return false;
    }

    if(lb)
    {
        printf("\nMEM LIST BODY | ERROR 3 LIST LONGER | %d", count_body);
        return false;
    }

    return true;
}

bool mem_list_body(list_body_p lb, ...)
{
    va_list args;
    va_start(args, lb);
    return mem_list_body_variadic(lb, &args);
}

bool mem_list_head(list_head_p lh, ...)
{
    va_list args;
    va_start(args, lh);

    int count_head = va_arg(args, int);

    int i=0;
    for(; lh && (i<count_head); lh = lh->lh, i++)
    {
        tag_t tag = va_arg(args, tag_t);
        if(!tag_eq(&lh->tag, &tag))
        {
            printf("\nMEM LIST HEAD | ERROR 1 TAG MISMATCH | %d %d", i, count_head);
            printf("\n\t(%s)", lh->tag.str);
            printf("\n\t(%s)", tag.str);
            printf("\n");
            return false;
        }

        if(!mem_list_body_variadic(lh->lb, &args))
        {
            printf("\nMEM LIST HEAD | ERROR 2 LIST BODY MISMATCH | %d %d", i, count_head);
            return false;
        }
    }

    if(i<count_head)
    {
        printf("\nMEM LIST HEAD | ERROR 3 LIST SHORTER | %d %d", i, count_head);
        return false;
    }

    if(lh)
    {
        printf("\nMEM LIST HEAD | ERROR 4 LIST LONGER | %d", count_head);
        return false;
    }

    return true;
}

tag_t tag_convert_test(char const format[], ...)
{
    va_list args;
    va_start(args, format);
    return tag_convert(format, args);
}

bool mem_list_head_insert_test(list_head_p *lh_root, handler_p h, char format[], ...)
{
    va_list args;
    va_start(args, format);
    return mem_list_head_insert(lh_root, h, format, args);
}

#endif

int mem_tag_len(char const tag_s[])
{
    int len = strlen(tag_s);
    assert(len < TAG_SIZE);
    return len;
}

tag_t mem_tag_convert_variadic(char const format[], va_list args)
{
    tag_t tag;
    memset(&tag, 0, TAG_SIZE);
    vsnprintf(tag.str, TAG_SIZE, format, args);
    return tag;
}

tag_t tag_convert(char const format[], ...)
{
    va_list args;
    va_start(args, format);

    return mem_tag_convert_variadic(format, args);
}


bool tag_eq(tag_p tag1, tag_p tag2)
{
    return memcmp(tag1, tag2, TAG_SIZE) == 0;
}



void mem_list_body_free(list_body_p lb)
{
    while(lb) lb = mem_list_body_pop(lb);
}

void mem_list_head_free(list_head_p *lh_root)
{
    for(list_head_p lh = *lh_root; lh; lh = mem_list_head_pop(lh))
        mem_list_body_free(lh->lb);

    *lh_root = NULL;
}

list_body_p mem_list_body_create(handler_p h)
{
    list_body_p lb = malloc(sizeof(list_body_t));
    assert(lb);
    INC(list_body);

    *lb = (list_body_t){h, NULL};
    return lb;
}

list_body_p mem_list_body_pop(list_body_p lb)
{
    assert(lb);
    list_body_p lb_aux = lb->lb;
    free(lb, list_body);
    return lb_aux;
}

bool mem_list_body_insert(list_body_p *lb_root, handler_p h)
{
    assert(lb_root);

    list_body_p lb = *lb_root;
    if(lb == NULL)
    {
        *lb_root = mem_list_body_create(h);
        return true;
    }

    if(lb->h == h)
        return false;

    return mem_list_body_insert(&lb->lb, h);
}

bool mem_list_body_remove(list_body_p *lb_root, handler_p h)
{
    assert(lb_root);

    list_body_p lb = *lb_root;
    if(lb == NULL) return false;

    if(lb->h != h)
        return mem_list_body_remove(&lb->lb, h);
    
    *lb_root = mem_list_body_pop(lb);
    return true;
}

int mem_list_body_count(list_body_p lb)
{
    int i = 0;
    for(; lb; i++, lb = lb->lb);
    return i;
}



list_head_p mem_list_head_create(tag_p tag, handler_p h)
{
    assert(h);

    list_head_p lh = calloc(1, sizeof(list_head_t));
    assert(lh);
    INC(list_head);

    list_body_p lb = mem_list_body_create(h);
    *lh = (list_head_t){NULL, lb, *tag};
    return lh;
}

list_head_p mem_list_head_pop(list_head_p lh)
{
    assert(lh);
    list_head_p lh_aux = lh->lh;
    free(lh, list_head);
    return lh_aux;
}



bool mem_list_head_insert_rec(list_head_p *lh_root, handler_p h, tag_p tag)
{
    assert(lh_root);

    list_head_p lh = *lh_root;
    if(lh == NULL)
    {
        *lh_root = mem_list_head_create(tag, h);
        return true;
    }

    if(tag_eq(&lh->tag, tag))
        return mem_list_body_insert(&lh->lb, h);

    return mem_list_head_insert_rec(&lh->lh, h, tag);
}

bool mem_list_head_insert(list_head_p *lh_root, handler_p h, char format[], va_list args)
{
    tag_t tag = mem_tag_convert_variadic(format, args);
    return mem_list_head_insert_rec(lh_root, h, &tag);
}

bool mem_list_head_remove(list_head_p *lh_root, handler_p h, tag_p tag)
{
    assert(lh_root);

    list_head_p lh = *lh_root;
    if(lh == NULL) return false;

    if(!mem_list_body_remove(&lh->lb, h))
        return mem_list_head_remove(&lh->lh, h, tag);

    if(tag) *tag = lh->tag;

    if(lh->lb == NULL)
        *lh_root = mem_list_head_pop(lh);

    return true;
}



void mem_list_report(list_head_p lh, char tag[])
{
    printf("\n\nMEM REPORT: %s", tag);
    if(lh == NULL)
    {
        printf("\n\nEMPTY LIST\n\n");
        return;
    }

    for(; lh; lh = lh->lh)
    {
        int count = mem_list_body_count(lh->lb);
        printf("\n%s: %d", lh->tag.str, count);
    }
    printf("\n\n");
}

void mem_list_report_full(list_head_p lh, char tag[])
{
    printf("\n\nMEM REPORT FULL: %s", tag);
    if(lh == NULL)
    {
        printf("\n\nEMPTY LIST\n\n");
        return;
    }

    for(; lh; lh = lh->lh)
    {
        printf("\n%s", lh->tag.str);
        for(list_body_p lb = lh->lb; lb; lb = lb->lb)
            printf("\n\t%p\t", lb->h);
    }
    printf("\n\n");
}



handler_p mem_list_get_pointer(list_head_p lh, int x, int y)
{
    for(int i=0; lh && (i < x); lh = lh->lh, i++);

    if(lh == NULL) return NULL;

    list_body_p lb = lh->lb;
    for(int j=0; lb && (j < x); lb = lb->lb, j++);
    
    return lb ? lb->h : NULL;
}
