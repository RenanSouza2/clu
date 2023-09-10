#include <stdio.h>
#include <stdlib.h>
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



int string_len(string_p str)
{
    int len = strlen(str);
    assert(len < STR_SIZE);
    return len;
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

list_head_p list_insert(list_head_p lh, handler_p h, string_p str)
{
    list_head_p lh_aux = list_head_find(lh, str);

    if(lh_aux == NULL)
        lh_aux = lh = list_head_create(str, lh);
    
    lh->lb = list_body_create(h, lh->lb);
    return lh;
}
