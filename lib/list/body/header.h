#ifndef __LIST_BODY_H__
#define __LIST_BODY_H__

#include <stdbool.h>
#include <stdint.h>

#include "../../../utils/struct.h"

PLACEHOLDER(list_body);

void clu_list_body_display(list_body_p lb);

void clu_list_body_free(list_body_p lb);

bool clu_list_body_insert(list_body_p *lb_root, handler_p h);
bool clu_list_body_remove(list_body_p *lb_root, handler_p h);

uint64_t clu_list_body_count(list_body_p lb);
handler_p clu_list_body_get_handler(list_body_p lb, uint64_t j);
bool clu_list_body_contains(list_body_p lb, handler_p h);

#endif
