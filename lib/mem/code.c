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

list_body_p list_body_create(handler_p h, list_body_p lb_next)
{
    list_body_p lb = malloc(sizeof(list_body_t));
    assert(lb);
    INC(list_body);

    *lb = (list_body_t){h, lb_next};
    return lb;
}

list_head_p list_head_create(string_t str, list_head_p lh_next)
{
    list_head_p lh = calloc(1, sizeof(list_head_t));
    assert(lh);
    INC(list_head);

    lh->lh = lh_next;

    int len = strlen(str);
    assert(len < STR_SIZE);
    
    memcpy(lh->str, str, len);
    return lh;
}

