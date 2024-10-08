#include <stdlib.h>

#include "debug.h"
#include "../../mem/header.h"
#include "../../../utils/assert.h"


#ifdef DEBUG

#include "../body/debug.h"



bool clu_list_head_test_immed(list_head_p lh, ...)
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

        if(!clu_list_body_test_variadic(lh->lb, &args))
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



void clu_list_report(list_head_p lh, char tag[], bool full)
{
    printf("\nMEM REPORT: %s", tag);
    if(lh == NULL)
    {
        printf("\n\nEMPTY LIST");
        return;
    }

    for(; lh; lh = lh->lh)
    {
        if(full)
        {
            printf("\n%s", lh->tag.str);
            clu_list_body_report_full(lh->lb);
        }
        else
        {
            int count = clu_list_body_count(lh->lb);
            printf("\n%s: %d", lh->tag.str, count);
        }
    }
}



handler_p clu_list_get_pointer(list_head_p lh, int x, int y) //  TODO test
{
    for(int i=0; i < x; lh = lh->lh, i++)
        if(lh == NULL)
            return NULL;

    return clu_list_body_get_pointer(lh->lb, y);
}
