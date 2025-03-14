#ifndef __LIST_HEAD_DEBUG_H__
#define __LIST_HEAD_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

#define LH(POINTER) ((list_head_p)(POINTER))

list_head_p clu_list_head_create_immed(uint64_t n, ...);

bool clu_list_head_immed(list_head_p lh, uint64_t n, ...);

#endif

list_head_p clu_list_head_create(tag_p tag);
list_head_p clu_list_head_pop(list_head_p lh);

#endif
