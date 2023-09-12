#ifndef __MEM_REPORT_H__
#define __MEM_REPORT_H__

#include "../../static_utils/struct.h"

PLACEHOLDER(list_head);

#define LH(POINTER) ((list_head_p)(POINTER))

list_head_p mem_list_insert(list_head_p lh, handler_p h, char const str[]);
void mem_list_remove(list_head_p lh, handler_p h);
void mem_list_report(list_head_p lh);

#endif
