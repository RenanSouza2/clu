#include <stdio.h>

#include "header.h"

#ifdef DEBUG

uint64_t list_alive;
uint64_t trie_alive;

void clu_mem_internal_display()
{
    printf("\n");
    printf("\n\tlist_alive: " U64P() "", list_alive);
    printf("\n\ttrie_alive: " U64P() "", trie_alive);
    printf("\n");
}

bool clu_mem_internal_empty()
{
    if(list_alive != 0 || trie_alive != 0)
    {
        clu_mem_internal_display();
        return false;
    }

    return true;
}

#endif
