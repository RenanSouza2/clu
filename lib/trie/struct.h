#ifndef __TRIE_STRUCT_H__
#define __TRIE_STRUCT_H__

#include "../../mods/macros/struct.h"
#include "../../mods/macros/uint.h"

[[maybe_unused]] constexpr uint64_t CHUNK = 4;
[[maybe_unused]] constexpr uint64_t SIZE = 16;
[[maybe_unused]] constexpr uint64_t INDEX_MAX = 16;
[[maybe_unused]] constexpr uint64_t MASK = U64(0xf);
[[maybe_unused]] constexpr uint64_t SH = 2;

#define OFFSET(INDEX) (U64(INDEX) << SH)
#define GET(HANDLER, INDEX) ((U64(HANDLER) >> OFFSET(INDEX)) & MASK)
#define SET(HANDLER, INDEX, KEY) ((handler_p)((U64(HANDLER) & ~(MASK << OFFSET(INDEX))) | U64(KEY) << OFFSET(INDEX)))

STRUCT(trie)
{
    handler_p h;
    uint64_t size;
    trie_p arr[SIZE];
};

#endif
