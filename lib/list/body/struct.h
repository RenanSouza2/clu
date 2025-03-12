#ifndef __LIST_BODY_STRUCT_H__
#define __LIST_BODY_STRUCT_H__

#include "header.h"

#define SIZE 256
#define CHUNK 8
#define INDEX_MAX 8

STRUCT(list_body)
{
    handler_p h;
    list_body_p arr[SIZE];
};

#endif
