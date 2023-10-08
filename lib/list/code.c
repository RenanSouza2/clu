#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "debug.h"

#ifdef DEBUG

int list_head_alive;
int list_body_alive;

bool list_memory()
{
    return list_head_alive == 0 && list_body_alive == 0;
}

#endif



int string_len(char const str[])
{
    int len = strlen(str);
    assert(len < STR_SIZE);
    return len;
}

void string_convert(string_p str, char const str_s[])
{
    int len = string_len(str_s);
    memset(str, 0, STR_SIZE);
    memcpy(str, str_s, len);
}



list_body_p mem_list_body_create(handler_p h, list_body_p lb_next)
{
    list_body_p lb = malloc(sizeof(list_body_t));
    assert(lb);
    INC(list_body);

    *lb = (list_body_t){h, lb_next};
    return lb;
}

list_body_p mem_list_body_pop(list_body_p lb)
{
    assert(lb);
    list_body_p lb_aux = lb->lb;
    free(lb, list_body);
    return lb_aux;
}

list_body_p mem_list_body_find(list_body_p lb, handler_p h)
{
    assert(lb);
    int i=0;
    for(; lb->lb; lb = lb->lb)
    if(lb->lb->h == h){
        printf("\n\tposition %d", i);
        return lb;
}
    return NULL;
}

bool mem_list_body_remove(list_body_p lb, handler_p h)
{
    assert(lb);
    lb = mem_list_body_find(lb, h);
    if(lb == NULL) return false;
    
    lb->lb = mem_list_body_pop(lb->lb);
    return true;
}

int mem_list_body_count(list_body_p lb)
{
    int i = 0;
    for(; lb; i++, lb = lb->lb)
        printf("\n%d: %p", i, lb);
    return i;
}



list_head_p mem_list_head_create(string_p str, list_head_p lh_next)
{
    list_head_p lh = calloc(1, sizeof(list_head_t));
    assert(lh);
    INC(list_head);

    lh->lh = lh_next;

    int len = string_len(str);
    
    memcpy(lh->str, str, len);
    return lh;
}

list_head_p mem_list_head_pop(list_head_p lh)
{
    assert(lh);
    list_head_p lh_aux = lh->lh;
    free(lh, list_head);
    return lh_aux;
}

list_head_p mem_list_head_find(list_head_p lh, string_p str)
{
    for(; lh; lh = lh->lh)
        if(strcmp(lh->str, str) == 0)
            break;

    return lh;
}



list_head_p mem_list_head_insert(list_head_p lh, handler_p h, char const str_s[])
{
    string_p str;
    string_convert(str, str_s);
    list_head_p lh_aux = mem_list_head_find(lh, str);

    if(lh_aux == NULL)
        lh_aux = lh = mem_list_head_create(str, lh);
    
    lh_aux->lb = mem_list_body_create(h, lh_aux->lb);
    return lh;
}

void mem_list_head_remove(list_head_p lh, handler_p h)
{
    assert(lh);
    printf("\nTrying to remove %p", h);
    for(; lh->lh; lh = lh->lh)
    {
        printf("\nTest %s", lh->str);
        if(mem_list_body_remove(LB(lh->lh), h))
        {
            printf("\tSuccess");
            break;
        }
        printf("\nFail");
    }

    if(lh->lh)
    if(lh->lh->lb == NULL)
        lh->lh = mem_list_head_pop(lh->lh);
}



void mem_list_report(list_head_p lh)
{
    printf("\n\nMEM REPORT");
    if(lh == NULL)
    {
        printf("\n\nEMPTY LIST\n\n");
        return;
    }

    for(; lh; lh = lh->lh)
    {
        int count = mem_list_body_count(lh->lb);
        printf("\n%s: %d", lh->str, count);
    }
    printf("\n\n");
}

void mem_list_report_full(list_head_p lh)
{
    printf("\n\nMEM REPORT");
    if(lh == NULL)
    {
        printf("\n\nEMPTY LIST\n\n");
        return;
    }

    for(; lh; lh = lh->lh)
    {
        printf("\n%s", lh->str);
        for(list_body_p lb = lh->lb; lb; lb = lb->lb)
            printf("\n\t%p", lb->h);
    }
    printf("\n\n");
}


handler_p mem_list_get_pointer(list_head_p lh, int x, int y)
{
    for(int i=0; lh && (i < x); lh = lh->lh, i++);

    if(lh == NULL) return NULL;

    list_body_p lb = lh->lb;
    for(int j=0; lb && (j < x); lb = lb->lb, j++);
    
    return lb ? lb->h : NULL;
}
