#ifndef __LIST_BODY_STRUCT_H__
#define __LIST_BODY_STRUCT_H__

#include "header.h"

#define SIZE 16
#define CHUNK 4
#define INDEX_MAX 16

STRUCT(list_body)
{
    list_body_p arr[SIZE];
};

#endif
