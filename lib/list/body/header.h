#ifndef __TRIE_H__
#define __TRIE_H__

#include <stdbool.h>
#include <stdint.h>

#include "../../../mods/macros/struct.h"

PLACEHOLDER(trie);

void clu_trie_display(trie_p t);

void clu_trie_free(trie_p t);

bool clu_trie_insert(trie_p *t_root, handler_p h);
bool clu_trie_remove(trie_p *t_root, handler_p h);

uint64_t clu_trie_count(trie_p t);
handler_p clu_trie_get_handler(trie_p t, uint64_t j);
bool clu_trie_contains(trie_p t, handler_p h);

#endif
