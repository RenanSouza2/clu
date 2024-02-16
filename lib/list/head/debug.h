#ifndef __LIST_HEAD_DEBUG_H__
#define __LIST_HEAD_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

#define LH(POINTER) ((list_head_p)(POINTER))

bool clu_list_body_test_immed(list_body_p lb, ...);
bool clu_list_head(list_head_p lh, ...);
bool clu_list_head_insert_immed(list_head_p *lh_root, handler_p h, char format[], ...);

#endif

bool clu_tag_eq(tag_p tag1, tag_p tag2);

list_body_p clu_list_body_create(handler_p h);
list_body_p clu_list_body_pop(list_body_p lb);

void clu_list_body_free(list_body_p lb);

bool clu_list_body_insert(list_body_p *lb_root, handler_p h);
bool clu_list_body_remove(list_body_p *lb, handler_p h);

list_head_p clu_list_head_create(tag_p tag, handler_p h);
list_head_p clu_list_head_pop(list_head_p lh);
list_head_p clu_list_head_find(list_head_p lh, tag_p tag);

#endif
