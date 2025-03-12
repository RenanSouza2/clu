#ifndef __LIST_BODY_STRUCT_H__
#define __LIST_BODY_STRUCT_H__

#include "header.h"

#define CHUNK 4
#define SIZE 16
#define INDEX_MAX 16
#define MASK U64(0xf)
#define SH 2

#define U64(HANDLER) ((uint64_t)(HANDLER))
#define OFFSET(INDEX) (((INDEX_MAX-1) - U64(INDEX)) << SH)
#define GET(HANDLER, INDEX) ((U64(HANDLER) >> OFFSET(INDEX)) & MASK)
#define SET(HANDLER, INDEX, KEY) ((handler_p)((U64(HANDLER) & ~(MASK << OFFSET(INDEX))) | U64(KEY) << OFFSET(INDEX)))


STRUCT(list_body)
{
    handler_p h;
    list_body_p arr[SIZE];
};

#endif
