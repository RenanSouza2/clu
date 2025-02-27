#ifndef __LIST_HEAD_H__
#define __LIST_HEAD_H__

#include <stdbool.h>

#include "../../../utils/struct.h"

#include "../../tag/header.h"

PLACEHOLDER(list_head);

void clu_list_head_free(list_head_p *lh_root);
bool clu_list_head_insert(list_head_p *lh_root, tag_p tag, handler_p h);
bool clu_list_head_remove(list_head_p *lh_root, handler_p h);

void clu_list_report(list_head_p lh, char tag[], bool full);

int clu_list_head_count(list_head_p lh);
int clu_list_head_count_tag(list_head_p lh, int x);
handler_p clu_list_get_pointer(list_head_p lh, int x, int y);
bool clu_list_head_contains(list_head_p lh, handler_p h);

#endif
