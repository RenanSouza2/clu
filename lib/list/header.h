#ifndef __LIST_H__
#define __LIST_H__

#include <stdbool.h>

#include "../trie/header.h"
#include "../tag/header.h"

#include "../../mods/macros/struct.h"

PLACEHOLDER(list)

void clu_list_report(list_p l, char const tag[], bool full);

void clu_list_free(list_p l_root);
bool clu_list_insert(list_p *l_root, tag_p tag, handler_p h);
bool clu_list_remove(list_p *l_root, handler_p h);

uint64_t clu_list_count(list_p l);
trie_p clu_list_get_trie(list_p l, uint64_t i);
bool clu_list_contains(list_p l, handler_p h);
tag_t clu_list_get_tag(list_p l, handler_p h);

#endif
