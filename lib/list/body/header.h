#ifndef __LIST_BODY_H__
#define __LIST_BODY_H__

#include "../../../utils/struct.h"

PLACEHOLDER(list_body);

int clu_list_body_count(list_body_p lb);

void clu_list_body_report_full(list_body_p lb);

handler_p clu_list_body_get_pointer(list_body_p lb, int y);

#endif
