#ifndef __LIST_STRUCT_H__
#define __LIST_STRUCT_H__

#include "header.h"

#define STR_SIZE 50
typedef char string_p[STR_SIZE];

STRUCT(list_body)
{
    handler_p h;
    list_body_p lb;
};

STRUCT(list_head)
{
    list_head_p lh;
    list_body_p lb;
    string_p str;
};

#endif
