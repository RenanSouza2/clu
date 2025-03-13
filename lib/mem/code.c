#include <stdio.h>

#include "header.h"

#ifdef DEBUG

uint64_t list_head_alive;
uint64_t list_body_alive;

void clu_mem_internal_display()
{
    printf("\n");
    printf("\n\tlist_head_alive: " U64P() "", list_head_alive);
    printf("\n\tlist_body_alive: " U64P() "", list_body_alive);
    printf("\n");
}

bool clu_mem_internal_empty()
{
    if(list_head_alive != 0 || list_body_alive != 0)
    {
        clu_mem_internal_display();
        return false;
    }

    return true;
}

#endif
