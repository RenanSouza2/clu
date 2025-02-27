#ifndef __LIST_BODY_DEBUG_H__
#define __LIST_BODY_DEBUG_H__

#include <stdbool.h>

#include "struct.h"

#ifdef DEBUG

#include <stdarg.h>

#define LB(POINTER) ((list_body_p)(POINTER))

list_body_p clu_list_body_create_variadic(va_list *args);
list_body_p clu_list_body_create_immed(int n, ...);

bool clu_list_body_variadic(list_body_p lb, va_list *args);
bool clu_list_body_immed(list_body_p lb, ...);

void clu_list_body_display(list_body_p lb);

#endif

list_body_p clu_list_body_create(handler_p h);
list_body_p clu_list_body_pop(list_body_p lb);
void clu_list_body_free(list_body_p lb);

bool clu_list_body_insert(list_body_p *lb_root, handler_p h);
bool clu_list_body_remove(list_body_p *lb_root, handler_p h);

#endif
