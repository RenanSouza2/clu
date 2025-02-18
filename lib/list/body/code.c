#include <stdlib.h>

#include "debug.h"
#include "../../mem/header.h"
#include "../../../utils/assert.h"



#ifdef DEBUG

#include <stdio.h>
#include <stdarg.h>



bool clu_list_body_test_immed(list_body_p lb, ...)
{
    va_list args;
    va_start(args, lb);
    return clu_list_body_test_variadic(lb, &args);
}

bool clu_list_body_test_variadic(list_body_p lb, va_list *args)
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

#endif



list_body_p clu_list_body_create(handler_p h)
{
    list_body_p lb = malloc(sizeof(list_body_t));
    assert(lb);
    INC(list_body);

    *lb = (list_body_t){h, NULL};
    return lb;
}

list_body_p clu_list_body_pop(list_body_p lb)
{
    assert(lb);
    list_body_p lb_aux = lb->lb;
    free(lb, list_body);
    return lb_aux;
}

void clu_list_body_free(list_body_p lb)
{
    while(lb) lb = clu_list_body_pop(lb);
}



bool clu_list_body_insert(list_body_p *lb_root, handler_p h)
{
    assert(lb_root);

    list_body_p lb = *lb_root;
    if(lb == NULL)
    {
        *lb_root = clu_list_body_create(h);
        return true;
    }

    if(lb->h == h)
        return false;

    return clu_list_body_insert(&lb->lb, h);
}

bool clu_list_body_remove(list_body_p *lb_root, handler_p h)
{
    assert(lb_root);

    list_body_p lb = *lb_root;
    if(lb == NULL) return false;

    if(lb->h != h)
        return clu_list_body_remove(&lb->lb, h);
    
    *lb_root = clu_list_body_pop(lb);
    return true;
}

int clu_list_body_count(list_body_p lb)
{
    int i = 0;
    for(; lb; i++, lb = lb->lb);
    return i;
}



void clu_list_body_report_full(list_body_p lb)
{
    for(; lb; lb = lb->lb)
        printf("\n\t%p\t", lb->h);
}



handler_p clu_list_body_get_pointer(list_body_p lb, int y) // TODO test
{
    for(int j=0; j < y; lb = lb->lb, j++)
        if(lb == NULL)
            return NULL;

    return lb->h;
}

bool clu_list_body_contains(list_body_p lb, handler_p h) // TODO test
{
    for(; lb; lb = lb->lb)
        if(lb->h == h)
            return true;

    return false;
}
