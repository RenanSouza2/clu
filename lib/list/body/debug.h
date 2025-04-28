#ifndef __LIST_BODY_DEBUG_H__
#define __LIST_BODY_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

#include <stdarg.h>

#define HD(HANDLER) ((handler_p)(HANDLER))
#define LB(HANDLER) ((trie_p)(HANDLER))

void clu_trie_display_dbg(trie_p t);

trie_p clu_trie_create_immed_tree(int empty, ...);

trie_p clu_trie_create_variadic_list(uint64_t n, va_list *args);
trie_p clu_trie_create_immed_list(uint64_t n, ...);

bool uint64(uint64_t i1, uint64_t i2);
bool clu_trie_inner(trie_p t_1, trie_p t_2);
bool clu_trie_immed_tree(trie_p t, int content, ...);
bool clu_trie_immed_list(trie_p t, uint64_t n, ...);

#endif

trie_p clu_trie_create(handler_p h);

#endif
