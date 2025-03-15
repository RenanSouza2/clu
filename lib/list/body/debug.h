#ifndef __LIST_BODY_DEBUG_H__
#define __LIST_BODY_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

#include <stdarg.h>

#define LB(POINTER) ((list_body_p)(POINTER))

list_body_p clu_list_body_create_variadic(va_list *args);
list_body_p clu_list_body_create_immed(bool empty, ...);
void clu_list_body_create_immed_vec(list_body_p lb[], uint64_t n, ...);
list_body_p clu_list_body_create_immed_list(uint64_t n, ...);

bool uint64(uint64_t i1, uint64_t i2);
bool clu_list_body_str(list_body_p lb_1, list_body_p lb_2);
bool clu_list_body_variadic(list_body_p lb, va_list *args);
bool clu_list_body_immed(list_body_p lb, ...);

#endif

list_body_p clu_list_body_create(handler_p h);

#endif
