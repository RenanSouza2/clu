#ifndef __LIST_STRUCT_H__
#define __LIST_STRUCT_H__

#include "header.h"

#define TAG_SIZE 50

STRUCT(tag)
{
    char str[TAG_SIZE];
};

STRUCT(list_body)
{
    handler_p h;
    list_body_p lb;
};

STRUCT(list_head)
{
    list_head_p lh;
    list_body_p lb;
    tag_t tag;
};

#endif
