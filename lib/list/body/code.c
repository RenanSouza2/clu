#include <stdlib.h>

#include "debug.h"
#include "../../mem/header.h"
#include "../../../utils/assert.h"



#ifdef DEBUG

#include <stdio.h>
#include <stdarg.h>



list_body_p clu_list_body_create_variadic_item(va_list *args)
{
    handler_p h = va_arg(*args, handler_p);
    return clu_list_body_create(h);
}

list_body_p clu_list_body_create_variadic_n(int n, va_list *args)
{
    if(n == 0)
        return NULL;

    list_body_p lb, lb_first;
    lb = lb_first = clu_list_body_create_variadic_item(args);
    for(int i=1; i<n; i++)
        lb = lb->lb = clu_list_body_create_variadic_item(args);

    return lb_first;
}

list_body_p clu_list_body_create_variadic(va_list *args)
{
    int n = va_arg(*args, int);
    return clu_list_body_create_variadic_n(n, args);
}

list_body_p clu_list_body_create_immed(int n, ...)
{
    va_list args;
    va_start(args, n);
    return clu_list_body_create_variadic_n(n, &args);
}



bool int_t(int i1, int i2)
{
    if(i1 != i2)
    {
        printf("\n\tINT ASSERTION ERROR\t| %d %d", i1, i2);
        return false;
    }

    return true;
}

bool clu_list_body_str(list_body_p lb_1, list_body_p lb_2)
{
    for(int i=0; lb_1 && lb_2; i++)
    {
        if(lb_1->h != lb_2->h)
        {
            printf("\n\tLIST BODY | ERROR 1 HANDLER MISMATCH | INDEX %d ", i);
            return false;
        }

        lb_1 = lb_1->lb;
        lb_2 = lb_2->lb;
    }

    if(lb_2)
    {
        printf("\n\tLIST BODY | ERROR 2 LIST SHORTER");
        return false;
    }

    if(lb_1)
    {
        printf("\n\tLIST BODY | ERROR 3 LIST LONGER");
        return false;
    }

    return true;
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

    if(index == 64)
    {
        printf("\n\t%p\t", h);
        return;
    }

    for(uint64_t i=0; i<16; i++)
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

    for(uint64_t i=0; i<16; i++)
        clu_list_body_free(lb->arr[i]);

    free(lb, list_body);
}



list_body_p clu_list_body_insert_rec_2(handler_p h, uint64_t index)
{
    list_body_p lb = clu_list_body_create();

    if(index < 64)
        lb->arr[GET(h, index)] = clu_list_body_insert_rec_2(h, index + 1);
    
    return lb;
}

bool clu_list_body_insert_rec_1(list_body_p *lb_root, handler_p h, uint64_t index)
{
    assert(lb_root);

    if(index == 64)
        return false;

    list_body_p lb = *lb_root;
    uint64_t key = GET(h, index);
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

    if(index == 64)
    {
        free(lb, list_body);
        *lb_root = NULL;
        return true;
    }

    if(!clu_list_body_remove_rec(&lb->arr[GET(h, index)], h, index + 1))
        return false;

    for(uint64_t i=0; i<16; i++)
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

    if(index == 64)
        return 1;

    uint64_t count = 0;
    for(uint64_t i=0; i<16; i++)
        count += clu_list_body_count_res(lb->arr[i], index + 1);

    return count;
}

uint64_t clu_list_body_count(list_body_p lb)
{
    return clu_list_body_count_res(lb, 0);
}

handler_p clu_list_body_get_handler_rec(list_body_p lb, int y, handler_p h, uint64_t index)
{
    assert(lb);

    if(index == 64)
        return h;

    for(uint64_t i=0; i<16; i++)
    {
        uint64_t count = clu_list_body_count_res(lb->arr[i], index + 1);
        if(y < count)
            return clu_list_body_get_handler_rec(lb->arr[i], y, SET(h, index, i),index + 1);

        y -= count;
    }
    assert(false);
}

handler_p clu_list_body_get_handler(list_body_p lb, int y)
{
    assert(lb);

    return clu_list_body_get_handler_rec(lb, y, NULL, 0);
}

bool clu_list_body_contains_rec(list_body_p lb, handler_p h, uint64_t index)
{
    if(lb == NULL)
        return false;

    if(index == 64)
        return true;

    return clu_list_body_contains_rec(lb->arr[GET(h, index)], h, index + 1);
}

bool clu_list_body_contains(list_body_p lb, handler_p h)
{
    return clu_list_body_contains_rec(lb, h, 0);
}
