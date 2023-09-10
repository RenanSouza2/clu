#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "debug.h"

#ifdef DEBUG

int list_head_count;
int list_body_count;

bool list_memory()
{
    return list_head_count == 0 && list_body_count == 0;
}

#endif



int string_len(char str[])
{
    int len = strlen(str);
    assert(len < STR_SIZE);
    return len;
}

void string_convert(string_p str, char str_s[])
{
    int len = string_len(str_s);
    memset(str, 0, STR_SIZE);
    memcpy(str, str_s, len);
}



list_body_p list_body_create(handler_p h, list_body_p lb_next)
{
    list_body_p lb = malloc(sizeof(list_body_t));
    assert(lb);
    INC(list_body);

    *lb = (list_body_t){h, lb_next};
    return lb;
}

list_body_p list_body_pop(list_body_p lb)
{
    list_body_p lb_aux = lb->lb;
    free(lb, list_body);
    return lb_aux;
}

bool list_body_remove(list_body_p lb, handler_p h)
{
    for(; lb->lb; lb = lb->lb)
    if(lb->lb->h == h)
    {
        lb->lb = list_body_pop(lb->lb);
        return true;
    }

    return false;
}



list_head_p list_head_create(string_p str, list_head_p lh_next)
{
    list_head_p lh = calloc(1, sizeof(list_head_t));
    assert(lh);
    INC(list_head);

    lh->lh = lh_next;

    int len = string_len(str);
    
    memcpy(lh->str, str, len);
    return lh;
}

list_head_p list_head_pop(list_head_p lh)
{
    list_head_p lh_aux = lh->lh;
    free(lh, list_head);
    return lh_aux;
}

list_head_p list_head_find(list_head_p lh, string_p str)
{
    for(; lh; lh = lh->lh)
        if(strcmp(lh->str, str) == 0)
            break;

    return lh;
}



list_head_p list_insert(list_head_p lh, handler_p h, char str_s[])
{
    string_p str;
    string_convert(str, str_s);
    list_head_p lh_aux = list_head_find(lh, str);

    if(lh_aux == NULL)
        lh_aux = lh = list_head_create(str, lh);
    
    lh->lb = list_body_create(h, lh->lb);
    return lh;
}

void list_remove(list_head_p lh, handler_p h)
{
    for(; lh->lh; lh = lh->lh)
    {
        if(!list_body_remove(LB(lh->lh), h)) continue;

        if(lh->lh->lb) return;

        lh->lh = list_head_pop(lh->lh);
        return;
    }

    assert(false);
}
