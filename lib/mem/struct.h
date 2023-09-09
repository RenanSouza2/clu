#ifndef __MEM_STRUCT_H__
#define __MEM_STRUCT_H__

#include "header.h"

#define STR_SIZE 50
typedef char string_t[STR_SIZE];

STRUCT(list_body)
{
    handler_p h;
    list_body_p lb;
};

STRUCT(list_head)
{
    list_body_p lb;
    string_t str;
    list_head_p lh;
};

#endif
