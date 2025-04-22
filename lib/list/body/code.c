#include <stdlib.h>

#include "debug.h"
#include "../../mem/header.h"
#include "../../../mods/macros/assert.h"
#include "../../../mods/macros/U64.h"



#ifdef DEBUG

#include <stdio.h>
#include <stdarg.h>

list_body_p clu_list_body_create_variadic_tree_rec(va_list *args)
{
    handler_p h = va_arg(*args, handler_p);
    list_body_p lb = clu_list_body_create(h);
    if(h)
        return lb;

    uint64_t n = va_arg(*args, uint64_t);
    assert(n > 0);
    assert(n <= SIZE);
    for(uint64_t i=0; i<n; i++)
    {
        uint64_t k = va_arg(*args, uint64_t);
        assert(k < SIZE);
        assert(lb->arr[k] == NULL);
        lb->arr[k] = clu_list_body_create_variadic_tree_rec(args);
    }
    return lb;
}

list_body_p clu_list_body_create_variadic_tree(bool content, va_list *args)
{
    if(!content)
        return NULL;

    return clu_list_body_create_variadic_tree_rec(args);
}

list_body_p clu_list_body_create_immed_tree(int content, ...)
{
    va_list args;
    va_start(args, content);
    return  clu_list_body_create_variadic_tree(content, &args);
}



list_body_p clu_list_body_create_variadic_list(uint64_t n, va_list *args)
{
    assert(n > 0);

    list_body_p lb = NULL;
    for(uint64_t i=0; i<n; i++)
    {
        handler_p h = va_arg(*args, handler_p);
        assert(clu_list_body_insert(&lb, h));
    }

    uint64_t n_remove = va_arg(*args, uint64_t);
    assert(n_remove < n);
    for(uint64_t i=0; i<n_remove; i++)
    {
        handler_p h = va_arg(*args, handler_p);
        assert(clu_list_body_remove(&lb, h));
    }
    return lb;
}

list_body_p clu_list_body_create_immed_list(uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    return clu_list_body_create_variadic_list(n, &args);
}



void clu_list_body_display_dbg_rec(list_body_p lb, uint64_t index)
{
    assert(lb);

    if(lb->h)
    {
        if(index) printf("\t");
        printf("h: %p", lb->h);
        return;
    }

    for(uint64_t i=0; i<SIZE; i++)
        if(lb->arr[i])
        {
            printf("\n");
            for(uint64_t k=0; k<index; k++)
                printf("\t");
            printf("i: " U64P() "", i);
            clu_list_body_display_dbg_rec(lb->arr[i], index + 1);
        }
}

void clu_list_body_display_dbg(list_body_p lb)
{
    if(lb == NULL)
    {
        printf("\nEMPTY LIST");
        return;
    }

    printf("\n");
    clu_list_body_display_dbg_rec(lb, 0);
    printf("\n");
}



bool uint64(uint64_t i1, uint64_t i2)
{
    if(i1 != i2)
    {
        printf("\n\n\tUINT64 ASSERT ERROR\t| " U64P() " " U64P() "", i1, i2);
        return false;
    }

    return true;
}

bool clu_list_body_rec(list_body_p lb_1, list_body_p lb_2, handler_p h, uint64_t index)
{
    if(lb_1 == NULL)
    {
        if(lb_2 != NULL)
        {
            printf("\n\n\tLIST BODY ASSERT ERROR\t| L1 EMPTY L2 NOT | H %p | I " U64P() "", h, index);
            return false;
        }

        return true;
    }

    if(lb_2 == NULL)
    {
        printf("\n\n\tLIST BODY ASSERT ERROR\t| L1 NOT EMPTY L2 IS | H %p | I " U64P() "", h, index);
        return false;
    }

    if(lb_1->h != lb_2->h)
    {
        printf("\n\n\tLIST BODY ASSERT ERROR\t| H MISMATCH 1 | %p %p | H %p | I " U64P() "", lb_1->h, lb_2->h, h, index);
        return false;
    }

    if(lb_1->h != NULL)
    {
        for(uint64_t i=0; i<index; i++)
            if(!uint64(GET(lb_1->h, i), GET(h, i)))
            {
                printf("\n\tLIST BODY ASSERT ERROR\t| H MISMATCH 2 | %p | H  %p | I " U64P() "", lb_1->h, h, index);
                return false;
            }

        for(uint64_t i=0; i<16; i++)
            if(lb_1->arr[i])
            {
                printf("\n\n\tLIST BODY ASSERT ERROR\t| L1 HAS H AND BRANCH | %p " U64P() " " U64P() "", h, i, index);
                return false;
            }

        return true;
    }

    for(uint64_t i=0; i<16; i++)
        if(!clu_list_body_rec(lb_1->arr[i], lb_2->arr[i], SET(h, index, i), index + 1))
            return false;

    return true;
}

bool clu_list_body_inner(list_body_p lb_1, list_body_p lb_2)
{
    if(!clu_list_body_rec(lb_1, lb_2, NULL, 0))
    {
        clu_list_body_display_dbg(lb_1);
        clu_list_body_display_dbg(lb_2);
        return false;
    }

    return true;
}

bool clu_list_body(list_body_p lb_1, list_body_p lb_2)
{
    if(!clu_list_body_inner(lb_1, lb_2))
        return false;

    clu_list_body_free(lb_1);
    clu_list_body_free(lb_2);
    return true;
}

bool clu_list_body_immed_tree(list_body_p lb, bool content, ...)
{
    va_list args;
    va_start(args, content);
    list_body_p lb_2 = clu_list_body_create_variadic_tree(content, &args);
    return clu_list_body(lb, lb_2);
}

bool clu_list_body_immed_list(list_body_p lb, uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    list_body_p lb_2 = clu_list_body_create_variadic_list(n, &args);
    return clu_list_body(lb, lb_2);
}

#endif



void clu_list_body_display_a(list_body_p lb) // TODO
{
    if(lb == NULL)
        return;

    if(lb->h)
    {
        printf("\n\t%p\t", lb->h);
        return;
    }

    for(uint64_t i=0; i<SIZE; i++)
        clu_list_body_display(lb->arr[i]);
}



list_body_p clu_list_body_create(handler_p h)
{
    list_body_p lb;
    CALLOC(lb, list_body);

    lb->h = h;
    return lb;
}

void clu_list_body_free(list_body_p lb)
{
    if(lb == NULL)
        return;

    for(uint64_t i=0; i<SIZE; i++)
        clu_list_body_free(lb->arr[i]);

    FREE(lb, list_body);
}



bool clu_list_body_insert_rec(list_body_p *lb_root, handler_p h, uint64_t index)
{
    list_body_p lb = *lb_root;
    uint64_t key = GET(h, index);

    if(lb == NULL)
    {
        *lb_root = clu_list_body_create(h);
        return true;
    }

    if(lb->h)
    {
        if(lb->h == h)
            return false;

        assert(clu_list_body_insert_rec(&lb->arr[GET(lb->h, index)], lb->h, index + 1));
        lb->h = NULL;
    }

    return clu_list_body_insert_rec(&lb->arr[key], h, index + 1);
}

bool clu_list_body_remove_rec(list_body_p *lb_root, handler_p h, uint64_t index)
{
    list_body_p lb = *lb_root;
    if(lb == NULL)
        return false;

    if(lb->h)
    {
        if(lb->h != h)
            return false;

        FREE(lb, list_body);
        *lb_root = NULL;
        return true;
    }

    if(!clu_list_body_remove_rec(&lb->arr[GET(h, index)], h, index + 1))
        return false;

    for(uint64_t i=0; i<SIZE; i++)
        if(lb->arr[i])
            return true;

    FREE(lb, list_body);
    *lb_root = NULL;
    return true;
}



bool clu_list_body_insert(list_body_p *lb_root, handler_p h)
{
    assert(lb_root);
    assert(h);

    return clu_list_body_insert_rec(lb_root, h, 0);
}

bool clu_list_body_remove(list_body_p *lb_root, handler_p h)
{
    assert(lb_root);
    assert(h);

    return clu_list_body_remove_rec(lb_root, h, 0);
}



handler_p clu_list_body_get_handler_rec(list_body_p lb, uint64_t j, bool revert)
{
    assert(lb);

    if(lb->h)
    {
        if(j == 0)
            return lb->h;

        assert(!revert);
        return NULL;
    }

    for(uint64_t i=0; i<SIZE; i++)
    {
        uint64_t count = clu_list_body_count(lb->arr[i]);
        if(j < count)
            return clu_list_body_get_handler_rec(lb->arr[i], j, true);

        j -= count;
    }

    assert(!revert);
    return NULL;
}

bool clu_list_body_contains_rec(list_body_p lb, handler_p h, uint64_t index)
{
    if(lb == NULL)
        return false;

    if(lb->h)
        return lb->h == h;

    return clu_list_body_contains_rec(lb->arr[GET(h, index)], h, index + 1);
}



uint64_t clu_list_body_count(list_body_p lb)
{
    if(lb == NULL)
        return 0;

    if(lb->h)
        return 1;

    uint64_t count = 0;
    for(uint64_t i=0; i<SIZE; i++)
        count += clu_list_body_count(lb->arr[i]);

    return count;
}

handler_p clu_list_body_get_handler(list_body_p lb, uint64_t j)
{
    assert(lb);

    return clu_list_body_get_handler_rec(lb, j, false);
}

bool clu_list_body_contains(list_body_p lb, handler_p h)
{
    assert(lb);
    assert(h);

    return clu_list_body_contains_rec(lb, h, 0);
}
