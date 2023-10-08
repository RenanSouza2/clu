#ifndef __MEM_REPORT_H__
#define __MEM_REPORT_H__

#include "../../static_utils/struct.h"

PLACEHOLDER(list_head);

void mem_list_head_insert(list_head_p *lh, handler_p h, char const tag_s[]);
void mem_list_head_remove(list_head_p *lh, handler_p h);

void mem_list_report(list_head_p lh, char title[]);
void mem_list_report_full(list_head_p lh, char title[]);
handler_p mem_list_get_pointer(list_head_p lh, int x, int y);

#endif
