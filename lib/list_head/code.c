#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "debug.h"
#include "../../utils/assert.h"


#ifdef DEBUG

#include "../mem/debug.h"

#include "../list_body/debug.h"

bool clu_list_head(list_head_p lh, ...)
{
    va_list args;
    va_start(args, lh);

    int count_head = va_arg(args, int);

    int i=0;
    for(; lh && (i<count_head); lh = lh->lh, i++)
    {
        tag_t tag = va_arg(args, tag_t);
        if(!clu_tag_eq(&lh->tag, &tag))
        {
            printf("\nMEM LIST HEAD | ERROR 1 TAG MISMATCH | %d %d", i, count_head);
            printf("\n\t(%s)", lh->tag.str);
            printf("\n\t(%s)", tag.str);
            printf("\n");
            return false;
        }

        if(!clu_list_body_variadic(lh->lb, &args))
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

#endif



list_head_p clu_list_head_create(tag_p tag, handler_p h)
{
    assert(h);

    list_head_p lh = calloc(1, sizeof(list_head_t));
    assert(lh);
    INC(list_head);

    list_body_p lb = clu_list_body_create(h);
    *lh = (list_head_t){NULL, lb, *tag};
    return lh;
}

list_head_p clu_list_head_pop(list_head_p lh)
{
    assert(lh);
    list_head_p lh_aux = lh->lh;
    free(lh, list_head);
    return lh_aux;
}

void clu_list_head_free(list_head_p *lh_root)
{
    for(list_head_p lh = *lh_root; lh; lh = clu_list_head_pop(lh))
        clu_list_body_free(lh->lb);

    *lh_root = NULL;
}



bool clu_list_head_insert(list_head_p *lh_root, handler_p h, tag_p tag)
{
    assert(lh_root);

    list_head_p lh = *lh_root;
    if(lh == NULL)
    {
        *lh_root = clu_list_head_create(tag, h);
        return true;
    }

    if(clu_tag_eq(&lh->tag, tag))
        return clu_list_body_insert(&lh->lb, h);

    return clu_list_head_insert(&lh->lh, h, tag);
}

bool clu_list_head_remove(list_head_p *lh_root, handler_p h)
{
    assert(lh_root);

    list_head_p lh = *lh_root;
    if(lh == NULL) return false;

    if(!clu_list_body_remove(&lh->lb, h))
        return clu_list_head_remove(&lh->lh, h);

    if(lh->lb == NULL)
        *lh_root = clu_list_head_pop(lh);

    return true;
}



void clu_list_report(list_head_p lh, char tag[])
{
    printf("\n\nMEM REPORT: %s", tag);
    if(lh == NULL)
    {
        printf("\n\nEMPTY LIST\n\n");
        return;
    }

    for(; lh; lh = lh->lh)
    {
        int count = clu_list_body_count(lh->lb);
        printf("\n%s: %d", lh->tag.str, count);
    }
    printf("\n\n");
}

void clu_list_report_full(list_head_p lh, char tag[])
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



handler_p clu_list_get_pointer(list_head_p lh, int x, int y)
{
    for(int i=0; lh && (i < x); lh = lh->lh, i++);

    if(lh == NULL) return NULL;

    list_body_p lb = lh->lb;
    for(int j=0; lb && (j < x); lb = lb->lb, j++);
    
    return lb ? lb->h : NULL;
}
