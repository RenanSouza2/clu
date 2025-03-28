#include <stdlib.h>

#include "debug.h"
#include "../body/header.h"
#include "../../mem/header.h"
#include "../../macros/assert.h"
#include "../../macros/U64.h"


#ifdef DEBUG

#include "../body/debug.h"
#include "../../tag/debug.h"

list_head_p clu_list_head_create_variadic_item(va_list *args)
{
    tag_t tag = va_arg(*args, tag_t);
    list_head_p lh = clu_list_head_create(&tag, NULL);
    lh->lb = clu_list_body_create_variadic_list(args);
    assert(lh->lb);
    return lh;
}

list_head_p clu_list_head_create_variadic_n(uint64_t n, va_list *args)
{
    if(n == 0)
        return NULL;

    list_head_p lh, lh_first;
    lh = lh_first = clu_list_head_create_variadic_item(args);
    for(uint64_t i=1; i<n; i++)
        lh = lh->lh = clu_list_head_create_variadic_item(args);

    return lh_first;
}

list_head_p clu_list_head_create_variadic(va_list *args)
{
    uint64_t n = va_arg(*args, uint64_t);
    return clu_list_head_create_variadic_n(n, args);
}

list_head_p clu_list_head_create_immed(uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    return clu_list_head_create_variadic_n(n, &args);
}

void clu_list_head_create_vec_immed(list_head_p lh[], uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    for(uint64_t i=0; i<n; i++)
        lh[i] = clu_list_head_create_variadic(&args);
}



bool clu_list_head_str(list_head_p lh_1, list_head_p lh_2)
{
    for(uint64_t i=0; lh_1 && lh_2; i++)
    {
        if(!clu_tag(&lh_1->tag, &lh_2->tag))
        {
            printf("\n\tLIST HEAD ASSERT ERROR\t| TAG MISMATCH | INDEX " U64P() "", i);
            return false;
        }

        if(!clu_list_body_str(lh_1->lb, lh_2->lb))
        {
            printf("\n\tLIST HEAD ASSERT ERROR\t| LIST BODY MISMATCH | " U64P() "", i);
            return false;
        }

        lh_1 = clu_list_head_pop(lh_1);
        lh_2 = clu_list_head_pop(lh_2);
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

bool clu_list_head_immed(list_head_p lh, uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    list_head_p lh_2 = clu_list_head_create_variadic_n(n, &args);
    return clu_list_head_str(lh, lh_2);
}

#endif



void clu_list_head_report(list_head_p lh, char tag[], bool full)
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
            uint64_t count = clu_list_body_count(lh->lb);
            printf("\n%s: " U64P() "", lh->tag.str, count);
        }
    }
}



list_head_p clu_list_head_create(tag_p tag, list_head_p next)
{
    list_head_p lh;
    CALLOC(lh, list_head);

    lh->tag = *tag;
    lh->lh = next;
    return lh;
}

list_head_p clu_list_head_pop(list_head_p lh)
{
    assert(lh);

    list_head_p lh_aux = lh->lh;
    FREE(lh, list_head);
    return lh_aux;
}



void clu_list_head_free(list_head_p lh_root)
{
    for(list_head_p lh = lh_root; lh; lh = clu_list_head_pop(lh))
        clu_list_body_free(lh->lb);
}

bool clu_list_head_insert(list_head_p *lh_root, tag_p tag, handler_p h)
{
    assert(lh_root);
    assert(tag);
    assert(h);

    if(clu_list_head_contains(*lh_root, h))
        return false;

    for(list_head_p lh = *lh_root; lh; lh = lh->lh)
    {
        if(!clu_tag_eq(&lh->tag, tag))
            continue;

        assert(clu_list_body_insert(&lh->lb, h));
        return true;
    }

    list_head_p lh = clu_list_head_create(tag, *lh_root);
    assert(clu_list_body_insert(&lh->lb, h));
    *lh_root = lh;
    return true;
}

bool clu_list_head_remove(list_head_p *lh_root, handler_p h)
{
    assert(lh_root);
    assert(h);

    list_head_p lh = *lh_root;
    if(lh == NULL) return false;

    if(!clu_list_body_remove(&lh->lb, h))
        return clu_list_head_remove(&lh->lh, h);

    if(lh->lb == NULL)
        *lh_root = clu_list_head_pop(lh);

    return true;
}



uint64_t clu_list_head_count(list_head_p lh)
{
    uint64_t i = 0;
    for(; lh; i++)
        lh = lh->lh;

    return i;
}

list_body_p clu_list_head_get_body(list_head_p lh, uint64_t i)
{
    if(lh == NULL)
        return NULL;

    if(i == 0)
        return lh->lb;

    return clu_list_head_get_body(lh->lh, i-1);
}

bool clu_list_head_contains(list_head_p lh, handler_p h)
{
    for(; lh; lh = lh->lh)
        if(clu_list_body_contains(lh->lb, h))
            return true;

    return false;
}
