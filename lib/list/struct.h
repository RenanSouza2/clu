#ifndef __LIST_STRUCT_H__
#define __LIST_STRUCT_H__

#include "../tag/struct.h"

STRUCT(list)
{
    tag_t tag;
    trie_p t;
    list_p next;
};

#endif
