#ifndef __LIST_BODY_DEBUG_H__
#define __LIST_BODY_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

#include <stdarg.h>

#define HD(HANDLER) ((handler_p)(HANDLER))
#define LB(HANDLER) ((list_body_p)(HANDLER))

void clu_list_body_display_dbg(list_body_p lb);

list_body_p clu_list_body_create_immed_tree(int empty, ...);

list_body_p clu_list_body_create_variadic_list(uint64_t n, va_list *args);
list_body_p clu_list_body_create_immed_list(uint64_t n, ...);

bool uint64(uint64_t i1, uint64_t i2);
bool clu_list_body_inner(list_body_p lb_1, list_body_p lb_2);
bool clu_list_body_immed_tree(list_body_p lb, int content, ...);
bool clu_list_body_immed_list(list_body_p lb, uint64_t n, ...);

#endif

list_body_p clu_list_body_create(handler_p h);

#endif
