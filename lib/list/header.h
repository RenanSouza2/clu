#ifndef __LIST_H__
#define __LIST_H__

#include "../../static_utils/struct.h"

PLACEHOLDER(list_head);

list_head_p list_insert(list_head_p lh, handler_p h, char str[]);
void list_remove(list_head_p lh, handler_p h);

#endif
