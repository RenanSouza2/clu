#ifndef __LIST_BODY_STRUCT_H__
#define __LIST_BODY_STRUCT_H__

#define CHUNK 4
#define SIZE 16
#define INDEX_MAX 16
#define MASK U64(0xf)
#define SH 2

#define OFFSET(INDEX) (U64(INDEX) << SH)
#define GET(HANDLER, INDEX) ((U64(HANDLER) >> OFFSET(INDEX)) & MASK)
#define SET(HANDLER, INDEX, KEY) ((handler_p)((U64(HANDLER) & ~(MASK << OFFSET(INDEX))) | U64(KEY) << OFFSET(INDEX)))

STRUCT(trie)
{
    handler_p h;
    trie_p arr[SIZE];
};

#endif
