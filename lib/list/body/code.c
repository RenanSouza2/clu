#include <stdlib.h>

#include "debug.h"
#include "../../mem/header.h"
#include "../../../utils/assert.h"



#ifdef DEBUG

#include <stdio.h>
#include <stdarg.h>

list_body_p clu_list_body_create_variadic_n(int n, va_list *args)
{
    if(n == 0)
        return NULL;

    handler_p h = va_arg(*args, handler_p);
    list_body_p lb_first = clu_list_body_create(h);

    list_body_p lb = lb_first;
    for(int i=1; i<n; i++)
    {
        h = va_arg(*args, handler_p);
        lb = lb->lb = clu_list_body_create(h);
    }
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



bool clu_list_body_str(list_body_p lb_1, list_body_p lb_2)
{
    for(int i=0; lb_1 && lb_2; i++)
    {
        if(lb_1->h != lb_2->h)
        {
            printf("\nMEM LIST BODY | ERROR 1 HANDLER MISMATCH | INDEX %d ", i);
            return false;
        }

        lb_1 = lb_1->lb;
        lb_2 = lb_2->lb;
    }

    if(lb_2)
    {
        printf("\nMEM LIST BODY | ERROR 2 LIST SHORTER");
        return false;
    }

    if(lb_1)
    {
        printf("\nMEM LIST BODY | ERROR 3 LIST LONGER");
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



void clu_list_body_display(list_body_p lb)
{
    printf("\n");
    for(; lb; lb = lb->lb)
        printf("\nlb->lh: %p", lb->h);
    printf("\n");
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
    while(lb)
        lb = clu_list_body_pop(lb);
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
