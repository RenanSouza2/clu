#ifndef __CLU_H__
#define __CLU_H__

#include <stdlib.h>

#ifdef DEBUG

#include <stdio.h>

#include "../../mods/macros/uint.h"

typedef void * handler_p;
[[gnu::malloc, gnu::alloc_size(1), gnu::format(printf, 2, 3)]]
handler_p clu_handler_malloc(size_t size, char const format[], ...);

[[gnu::malloc, gnu::alloc_size(1, 2), gnu::format(printf, 3, 4)]]
handler_p clu_handler_calloc(size_t amt, size_t size, char const format[], ...);

[[gnu::alloc_size(2), gnu::format(printf, 3, 4)]]
handler_p clu_handler_realloc(handler_p h, size_t size, char const format[], ...);

[[gnu::format(printf, 2, 3)]]
void clu_handler_free(handler_p h, char const format[], ...);

[[gnu::format(printf, 2, 3)]] void clu_handler_register(handler_p h, char const format[], ...);
[[gnu::format(printf, 2, 3)]] void clu_handler_unregister(handler_p h, char const format[], ...);
[[gnu::format(printf, 2, 3)]] void clu_handler_register_static(handler_p h, char const format[], ...);
[[gnu::format(printf, 2, 3)]] void clu_handler_is_safe(handler_p h, char const format[], ...);
bool clu_handler_is_static(handler_p h);

void clu_mem_report(char const tag[]);
void clu_mem_report_full(char const tag[]);
bool clu_mem_is_empty();

bool clu_handler_is_allocated(handler_p h);
bool clu_handler_is_freed(handler_p h);

uint64_t clu_get_max_i();
uint64_t clu_get_max_j(uint64_t i);
handler_p clu_get_handler(uint64_t i, uint64_t j);

[[maybe_unused]]
constexpr uint64_t CLU_LOG_DISABLED = 0;
[[maybe_unused]]
constexpr uint64_t CLU_LOG_DYNAMIC = 1;
[[maybe_unused]]
constexpr uint64_t CLU_LOG_ALL = 2;
void clu_log_level_set(uint64_t _clu_log_level);

uint64_t clu_get_occupancy();
uint64_t clu_get_max_occupancy();
void clu_clean_max_occupancy();
uint64_t clu_get_register_count();

#define CLU_TAG(FUNC, LINE) "f|%s|l|" U64P() "", FUNC, (uint64_t)LINE
#define CLU_DEFAULT_TAG CLU_TAG(__func__, __LINE__)
#define CLU_STACK_TAG CLU_TAG(func, line)

#define malloc(SIZE) clu_handler_malloc(SIZE, CLU_DEFAULT_TAG)
#define calloc(AMOUNT, SIZE) clu_handler_calloc(AMOUNT, SIZE, CLU_DEFAULT_TAG)
#define realloc(HANDLER, SIZE) clu_handler_realloc(HANDLER, SIZE, CLU_DEFAULT_TAG)
#define free(HANDLER) clu_handler_free(HANDLER, CLU_DEFAULT_TAG)

#define malloc_tag(SIZE, ...) clu_handler_malloc(SIZE, __VA_ARGS__)
#define calloc_tag(AMOUNT, SIZE, ...) clu_handler_calloc(AMOUNT, SIZE, __VA_ARGS__)

#define CLU_HANDLER_IS_SAFE(HANDLER) clu_handler_is_safe(HANDLER, CLU_DEFAULT_TAG);
#define CLU_HANDLER_REGISTER(HANDLER) clu_handler_register(HANDLER, CLU_DEFAULT_TAG);
#define CLU_HANDLER_UNREGISTER(HANDLER) clu_handler_unregister(HANDLER, CLU_DEFAULT_TAG);
#define CLU_HANDLER_REGISTER_STATIC(HANDLER) clu_handler_register_static(HANDLER, CLU_DEFAULT_TAG);

#define CLU_HANDLER_REGISTER_TAG(HANDLER, ...) clu_handler_register(HANDLER, __VA_ARGS__);

#define CLU_PARAMS(...) __VA_OPT__(__VA_ARGS__,) char const func[], uint64_t line
#define CLU_ARGS(...) __VA_OPT__(__VA_ARGS__,) __func__, __LINE__

#else // DEBUG

#define malloc_tag(SIZE, ...) malloc(SIZE)
#define calloc_tag(AMOUNT, SIZE, ...) calloc(AMOUNT, SIZE)

#define CLU_HANDLER_IS_SAFE(HANDLER)
#define CLU_HANDLER_REGISTER(HANDLER)
#define CLU_HANDLER_UNREGISTER(HANDLER)
#define CLU_HANDLER_REGISTER_STATIC(HANDLER)

#define CLU_HANDLER_REGISTER_TAG(HANDLER, ...)

#define CLU_PARAMS(...) __VA_ARGS__
#define CLU_ARGS(...) __VA_ARGS__

#endif // DEBUG

#endif
