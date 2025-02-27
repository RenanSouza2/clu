#ifndef __LIST_HEAD_DEBUG_H__
#define __LIST_HEAD_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

#define LH(POINTER) ((list_head_p)(POINTER))

list_head_p clu_list_head_create_immed(int n, ...);

bool clu_list_head_immed(list_head_p lh, int n, ...);

#endif

list_head_p clu_list_head_create(tag_p tag, list_body_p lb);
list_head_p clu_list_head_create_handler(tag_p tag, handler_p h);
list_head_p clu_list_head_pop(list_head_p lh);

list_body_p cli_list_head_get_body(list_head_p lh, int x);

#endif
