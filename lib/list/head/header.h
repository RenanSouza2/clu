#ifndef __LIST_HEAD_H__
#define __LIST_HEAD_H__

#include <stdbool.h>

#include "../body/header.h"
#include "../../tag/header.h"

#include "../../macros/struct.h"

PLACEHOLDER(list_head);

void clu_list_head_report(list_head_p lh, char tag[], bool full);

list_head_p clu_list_head_free(list_head_p lh_root);
bool clu_list_head_insert(list_head_p *lh_root, tag_p tag, handler_p h);
bool clu_list_head_remove(list_head_p *lh_root, handler_p h);

uint64_t clu_list_head_count(list_head_p lh);
list_body_p clu_list_head_get_body(list_head_p lh, uint64_t i);
bool clu_list_head_contains(list_head_p lh, handler_p h);

#endif
