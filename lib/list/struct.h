#ifndef LIST_STRUCT_H
#define LIST_STRUCT_H

#include "../tag/struct.h"
#include "../trie/struct.h"

STRUCT(list)
{
    tag_t tag;
    trie_p t;
    list_p next;
};

#endif
