#include <stdlib.h>

#include "debug.h"
#include "../../mem/header.h"
#include "../../../utils/assert.h"
#include "../../../utils/U64.h"



#ifdef DEBUG

#include <stdio.h>
#include <stdarg.h>



list_body_p clu_list_body_create_variadic_n(uint64_t n, va_list *args)
{
    if(n == 0)
        return NULL;

    list_body_p lb = NULL;
    for(uint64_t i=0; i<n; i++)
    {
        handler_p h = va_arg(*args, handler_p);
        assert(clu_list_body_insert(&lb, h));
    }

    return lb;
}

list_body_p clu_list_body_create_variadic(va_list *args)
{
    uint64_t n = va_arg(*args, uint64_t);
    return clu_list_body_create_variadic_n(n, args);
}

list_body_p clu_list_body_create_immed(uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    return clu_list_body_create_variadic_n(n, &args);
}



bool uint64(uint64_t i1, uint64_t i2)
{
    if(i1 != i2)
    {
        printf("\n\n\tUINT64 ASSERTION ERROR\t| " U64P() " " U64P() "", i1, i2);
        return false;
    }

    return true;
}

bool clu_list_body_str_rec(list_body_p lb_1, list_body_p lb_2, handler_p h, uint64_t index)
{
    if(lb_1 == NULL)
    {
        if(lb_2 != NULL)
        {
            printf("\n\n\tLIST BODY ASSERTION ERROR\t| L1 EMPTY L2 NOT | %p " U64P() "", h, index);
            return false;
        }

        return true;
    }

    if(lb_2 == NULL)
    {
        printf("\n\n\tLIST BODY ASSERTION ERROR\t| L1 NOT EMPTY L2 IS | %p " U64P() "", h, index);
        return false;
    }

    for(uint64_t i=0; i<16; i++)
        if(!clu_list_body_str_rec(lb_1->arr[i], lb_2->arr[i], SET(h, index, i), index + 1))
            return false;

    return true;
}

bool clu_list_body_str(list_body_p lb_1, list_body_p lb_2)
{
    return clu_list_body_str_rec(lb_1, lb_2, NULL, 0);
}

bool clu_list_body_variadic(list_body_p lb, va_list *args)
{
    list_body_p lb_2 = clu_list_body_create_variadic(args);
    bool res = clu_list_body_str(lb, lb_2);

    clu_list_body_free(lb_2);
    return res;
}

bool clu_list_body_immed(list_body_p lb, ...)
{
    va_list args;
    va_start(args, lb);
    bool res = clu_list_body_variadic(lb, &args);

    clu_list_body_free(lb);
    return res;
}

#endif



void clu_list_body_display_rec(list_body_p lb, handler_p h, uint64_t index)
{
    if(lb == NULL)
        return;

    if(index == INDEX_MAX)
    {
        printf("\n\t%p\t", h);
        return;
    }

    for(uint64_t i=0; i<SIZE; i++)
        clu_list_body_display_rec(lb->arr[i], SET(h, index, i), index + 1);
}

void clu_list_body_display(list_body_p lb)
{
    clu_list_body_display_rec(lb, NULL, 0);
}



list_body_p clu_list_body_create()
{
    list_body_p lb = calloc(1, sizeof(list_body_t));
    assert(lb);
    INC(list_body);

    return lb;
}

void clu_list_body_free(list_body_p lb)
{
    if(lb == NULL)
        return;

    for(uint64_t i=0; i<SIZE; i++)
        clu_list_body_free(lb->arr[i]);

    free(lb, list_body);
}



list_body_p clu_list_body_insert_rec_2(handler_p h, uint64_t index)
{
    list_body_p lb = clu_list_body_create();

    if(index < INDEX_MAX)
        lb->arr[GET(h, index)] = clu_list_body_insert_rec_2(h, index + 1);

    return lb;
}

bool clu_list_body_insert_rec_1(list_body_p *lb_root, handler_p h, uint64_t index)
{
    assert(lb_root);

    list_body_p lb = *lb_root;
    uint64_t key = GET(h, index);

    if(index == INDEX_MAX)
    {
        if(lb != NULL)
            return false;

        *lb_root = clu_list_body_create();
        return true;
    }

    if(lb != NULL)
        return clu_list_body_insert_rec_1(&lb->arr[key], h, index + 1);

    *lb_root = lb = clu_list_body_create();
    lb->arr[key] = clu_list_body_insert_rec_2(h, index + 1);
    return true;
}

bool clu_list_body_insert(list_body_p *lb_root, handler_p h)
{
    return clu_list_body_insert_rec_1(lb_root, h, 0);
}


bool clu_list_body_remove_rec(list_body_p *lb_root, handler_p h, uint64_t index)
{
    assert(lb_root);

    list_body_p lb = *lb_root;
    if(lb == NULL)
        return false;

    if(index == INDEX_MAX)
    {
        free(lb, list_body);
        *lb_root = NULL;
        return true;
    }

    if(!clu_list_body_remove_rec(&lb->arr[GET(h, index)], h, index + 1))
        return false;

    for(uint64_t i=0; i<SIZE; i++)
        if(lb->arr[i])
            return true;

    free(lb, list_body);
    *lb_root = lb = NULL;
    return true;
}

bool clu_list_body_remove(list_body_p *lb_root, handler_p h)
{
    assert(lb_root);

    return clu_list_body_remove_rec(lb_root, h, 0);
}



uint64_t clu_list_body_count_res(list_body_p lb, uint64_t index)
{
    if(lb == NULL)
        return 0;

    if(index == INDEX_MAX)
        return 1;

    uint64_t count = 0;
    for(uint64_t i=0; i<SIZE; i++)
        count += clu_list_body_count_res(lb->arr[i], index + 1);

    return count;
}

uint64_t clu_list_body_count(list_body_p lb)
{
    return clu_list_body_count_res(lb, 0);
}

handler_p clu_list_body_get_handler_rec(list_body_p lb, int y, handler_p h, uint64_t index, bool revert)
{
    assert(lb);

    if(index == INDEX_MAX)
        return h;

    for(uint64_t i=0; i<SIZE; i++)
    {
        uint64_t count = clu_list_body_count_res(lb->arr[i], index + 1);
        if(y < count)
            return clu_list_body_get_handler_rec(lb->arr[i], y, SET(h, index, i),index + 1, true);

        y -= count;
    }

    assert(!revert);
    return NULL;
}

handler_p clu_list_body_get_handler(list_body_p lb, int y)
{
    assert(lb);

    return clu_list_body_get_handler_rec(lb, y, NULL, 0, false);
}

bool clu_list_body_contains_rec(list_body_p lb, handler_p h, uint64_t index)
{
    if(lb == NULL)
        return false;

    if(index == INDEX_MAX)
        return true;

    return clu_list_body_contains_rec(lb->arr[GET(h, index)], h, index + 1);
}

bool clu_list_body_contains(list_body_p lb, handler_p h)
{
    return clu_list_body_contains_rec(lb, h, 0);
}
