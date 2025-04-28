#ifndef __LIST_HEAD_STRUCT_H__
#define __LIST_HEAD_STRUCT_H__

#include "../../tag/struct.h"

STRUCT(list_head)
{
    tag_t tag;
    trie_p t;
    list_head_p lh;
};

#endif
