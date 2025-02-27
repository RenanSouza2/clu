#include <stdlib.h>

#include "debug.h"
#include "../body/header.h"
#include "../../mem/header.h"
#include "../../../utils/assert.h"


#ifdef DEBUG

#include "../body/debug.h"
#include "../../tag/debug.h"

list_head_p clu_list_head_create_variadic_item(va_list *args)
{
    tag_t tag = va_arg(*args, tag_t);
    list_body_p lb = clu_list_body_create_variadic(args);
    assert(lb);
    return clu_list_head_create(&tag, lb);
}

list_head_p clu_list_head_create_variadic(int n, va_list *args)
{
    if(n == 0)
        return NULL;
    
    list_head_p lh, lh_first;
    lh = lh_first = clu_list_head_create_variadic_item(args);
    for(int i=1; i<n; i++)
        lh = lh->lh = clu_list_head_create_variadic_item(args);

    return lh_first;
}

list_head_p clu_list_head_create_immed(int n, ...)
{
    va_list args;
    va_start(args, n);
    return clu_list_head_create_variadic(n, &args);
}



bool clu_list_head_str(list_head_p lh_1, list_head_p lh_2)
{
    for(int i; lh_1 && lh_2; i++)
    {
        if(!clu_tag(&lh_1->tag, &lh_2->tag))
        {
            printf("\n\tLIST HEAD ASSERT ERROR\t| TAG MISMATCH | INDEX %d ", i);
            return false;
        }

        if(!clu_list_body_str(lh_1->lb, lh_2->lb))
        {
            printf("\n\tLIST HEAD ASSERT ERROR\t| LIST BODY MISMATCH | %d", i);
            return false;
        }

        lh_1 = lh_1->lh;
        lh_2 = lh_2->lh;
    }

    if(lh_2)
    {
        printf("\n\tLIST HEAD ASSERT ERROR\t| LIST SHORTER");
        return false;
    }

    if(lh_1)
    {
        printf("\n\tLIST HEAD ASSERT ERROR\t| LIST LONGER");
        return false;
    }

    return true;
}

bool clu_list_head_immed(list_head_p lh, int n, ...)
{
    va_list args;
    va_start(args, n);
    list_head_p lh_2 = clu_list_head_create_variadic(n, &args);

    bool res = clu_list_head_str(lh, lh_2);

    clu_list_head_free(&lh);
    clu_list_head_free(&lh_2);
    return res;
}

#endif



void clu_list_report(list_head_p lh, char tag[], bool full)
{
    printf("\n\tCLU REPORT: %s", tag);
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
            clu_list_body_display(lh->lb);
        }
        else
        {
            int count = clu_list_body_count(lh->lb);
            printf("\n%s: %d", lh->tag.str, count);
        }
    }
}



list_head_p clu_list_head_create(tag_p tag, list_body_p lb)
{
    list_head_p lh = calloc(1, sizeof(list_head_t));
    assert(lh);
    INC(list_head);

    *lh = (list_head_t)
    {
        .tag = *tag,
        .lb = lb, 
        .lh = NULL 
    };
    return lh;
}

list_head_p clu_list_head_create_handler(tag_p tag, handler_p h)
{
    assert(h);

    list_body_p lb = clu_list_body_create(h);
    list_head_p lh = clu_list_head_create(tag, lb);
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



bool clu_list_head_insert(list_head_p *lh_root, tag_p tag, handler_p h)
{
    assert(lh_root);
    list_head_p lh = *lh_root;
    if(lh == NULL)
    {
        *lh_root = clu_list_head_create_handler(tag, h);
        return true;
    }

    if(clu_tag_eq(&lh->tag, tag))
        return clu_list_body_insert(&lh->lb, h);

    return clu_list_head_insert(&lh->lh, tag, h);
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



int clu_list_head_count(list_head_p lh)
{
    int i = 0;
    for(; lh; i++)
        lh = lh->lh;

    return i;
}

list_body_p cli_list_head_get_body(list_head_p lh, int x)
{
    if(lh == NULL)
        return NULL;

    if(x == 0)
        return lh->lb;

    return cli_list_head_get_body(lh->lh, x-1);
}

bool clu_list_head_contains(list_head_p lh, handler_p h) // TODO test
{
    for(; lh; lh = lh->lh)
        if(clu_list_body_contains(lh->lb, h))
            return true;

    return false;
}
