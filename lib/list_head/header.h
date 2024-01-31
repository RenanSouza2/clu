#ifndef __LIST_HEAD_H__
#define __LIST_HEAD_H__

#include <stdarg.h>

#include "../../static_utils/struct.h"
#include "../tag/header.h"

PLACEHOLDER(list_head);

bool clu_list_head_insert(list_head_p *lh_root, handler_p h, char format[], va_list args);
void clu_list_head_free(list_head_p *lh_root);
bool clu_list_head_remove(list_head_p *lh_root, handler_p h, tag_p tag);

void clu_list_report(list_head_p lh, char title[]);
void clu_list_report_full(list_head_p lh, char title[]);
handler_p clu_list_get_pointer(list_head_p lh, int x, int y);

#endif
