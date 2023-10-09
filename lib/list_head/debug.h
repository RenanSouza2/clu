#ifndef __LIST_HEAD_DEBUG_H__
#define __LIST_HEAD_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

#define LH(POINTER) ((list_head_p)(POINTER))
#define HD(POINTER) ((handler_p)(POINTER))

bool mem_list_body(list_body_p lb, ...);
bool mem_list_head(list_head_p lh, ...);
bool mem_list_head_insert_test(list_head_p *lh_root, handler_p h, char format[], ...);

#endif

#define LB(POINTER) ((list_body_p)(POINTER))

bool mem_tag_eq(tag_p tag1, tag_p tag2);

list_body_p mem_list_body_create(handler_p h);
list_body_p mem_list_body_pop(list_body_p lb);

void mem_list_body_free(list_body_p lb);

bool mem_list_body_insert(list_body_p *lb_root, handler_p h);
bool mem_list_body_remove(list_body_p *lb, handler_p h);

list_head_p mem_list_head_create(tag_p tag, handler_p h);
list_head_p mem_list_head_pop(list_head_p lh);
list_head_p mem_list_head_find(list_head_p lh, tag_p tag);

#endif
