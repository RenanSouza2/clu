#include <stdio.h>

#include "debug.h"

#ifdef DEBUG

int list_head_alive;
int list_body_alive;

bool clu_mem_empty()
{
    return list_head_alive == 0 && list_body_alive == 0;
}

#endif
