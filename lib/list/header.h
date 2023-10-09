#ifndef __MEM_REPORT_H__
#define __MEM_REPORT_H__

#include <stdarg.h>

#include "../../static_utils/struct.h"

#define TAG_SIZE 50

STRUCT(tag)
{
    char str[TAG_SIZE];
};

PLACEHOLDER(list_head);

tag_t tag_convert(char const format[], ...);

bool mem_list_head_insert(list_head_p *lh_root, handler_p h, char format[], va_list args);
void mem_list_head_free(list_head_p *lh_root);
bool mem_list_head_remove(list_head_p *lh_root, handler_p h, tag_p tag);

void mem_list_report(list_head_p lh, char title[]);
void mem_list_report_full(list_head_p lh, char title[]);
handler_p mem_list_get_pointer(list_head_p lh, int x, int y);

#endif
