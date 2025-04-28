#ifndef __LIST_DEBUG_H__
#define __LIST_DEBUG_H__

#include "header.h"
#include "struct.h"

#ifdef DEBUG

#define L(HANDLER) ((list_p)(HANDLER))

list_p clu_list_create_immed(uint64_t n, ...);

bool clu_list(list_p lh_1, list_p lh_2);
bool clu_list_immed(list_p lh, uint64_t n, ...);

#endif

list_p clu_list_create(tag_p tag, list_p next);
list_p clu_list_pop(list_p lh);

#endif
