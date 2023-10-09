#ifndef __LIST_BODY_DEBUG_H__
#define __LIST_BODY_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

#include <stdarg.h>

#define LB(POINTER) ((list_body_p)(POINTER))

bool mem_list_body_variadic(list_body_p lb, va_list *args);
bool mem_list_body(list_body_p lb, ...);

#endif

list_body_p mem_list_body_create(handler_p h);
list_body_p mem_list_body_pop(list_body_p lb);
void mem_list_body_free(list_body_p lb);

bool mem_list_body_insert(list_body_p *lb_root, handler_p h);
bool mem_list_body_remove(list_body_p *lb_root, handler_p h);

#endif
