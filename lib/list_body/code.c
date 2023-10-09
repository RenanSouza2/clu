#include <stdlib.h>

#include "debug.h"
#include "../mem/debug.h"

#ifdef DEBUG

#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

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

#endif



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

void mem_list_body_free(list_body_p lb)
{
    while(lb) lb = mem_list_body_pop(lb);
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
