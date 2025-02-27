#ifndef __LIST_HEAD_DEBUG_H__
#define __LIST_HEAD_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

#define LH(POINTER) ((list_head_p)(POINTER))

bool clu_list_head_test_immed(list_head_p lh, ...);
bool clu_list_head_insert_immed(list_head_p *lh_root, handler_p h, char format[], ...);

#endif

list_head_p clu_list_head_create(tag_p tag, list_body_p lb);
list_head_p clu_list_head_create_handler(tag_p tag, handler_p h);
list_head_p clu_list_head_pop(list_head_p lh);
list_head_p clu_list_head_find(list_head_p lh, tag_p tag);

#endif
