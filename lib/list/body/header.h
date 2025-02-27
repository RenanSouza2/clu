#ifndef __LIST_BODY_H__
#define __LIST_BODY_H__

#include "../../../utils/struct.h"

PLACEHOLDER(list_body);

void clu_list_body_display(list_body_p lb);

list_body_p clu_list_body_create(handler_p h);
void clu_list_body_free(list_body_p lb);

bool clu_list_body_insert(list_body_p *lb_root, handler_p h);
bool clu_list_body_remove(list_body_p *lb_root, handler_p h);

int clu_list_body_count(list_body_p lb);
handler_p clu_list_body_get_pointer(list_body_p lb, int y);
bool clu_list_body_contains(list_body_p lb, handler_p h);

#endif
