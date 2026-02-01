#ifndef __CLU_H__
#define __CLU_H__

#include <stdlib.h>

#ifdef DEBUG

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

typedef void * handler_p;

handler_p clu_handler_malloc(size_t size, char const format[], ...);
handler_p clu_handler_calloc(size_t amt, size_t size, char const format[], ...);
handler_p clu_handler_realloc(handler_p h, size_t size, char const format[], ...);
void clu_handler_free(handler_p h, char const format[], ...);

void clu_handler_register(handler_p h, char const format[], ...);
void clu_handler_unregister(handler_p h, char const format[], ...);
void clu_handler_register_static(handler_p h, char const format[], ...);

void clu_handler_is_safe(handler_p h, char const format[], ...);
bool clu_handler_is_static(handler_p h);

void clu_mem_report(char const tag[]);
void clu_mem_report_full(char const tag[]);
bool clu_mem_is_empty(void);

bool clu_handler_is_allocated(handler_p h);
bool clu_handler_is_freed(handler_p h);

uint64_t clu_get_max_i(void);
uint64_t clu_get_max_j(uint64_t i);
handler_p clu_get_handler(uint64_t i, uint64_t j);

#define CLU_LOG_DISABLED 0
#define CLU_LOG_DYNAMIC 1
#define CLU_LOG_ALL 2
void clu_log_level_set(uint64_t _clu_log_level);

uint64_t clu_get_occupancy(void);
uint64_t clu_get_max_occupancy(void);
void clu_clean_max_occupancy(void);
uint64_t clu_get_register_count(void);

#define CLU_DEFAULT_TAG "f|%s|l|%d", __func__, __LINE__

#define malloc(SIZE) clu_handler_malloc(SIZE, CLU_DEFAULT_TAG)
#define calloc(AMOUNT, SIZE) clu_handler_calloc(AMOUNT, SIZE, CLU_DEFAULT_TAG)
#define realloc(HANDLER, SIZE) clu_handler_realloc(HANDLER, SIZE, CLU_DEFAULT_TAG)
#define free(HANDLER) clu_handler_free(HANDLER, CLU_DEFAULT_TAG)

#define CLU_HANDLER_IS_SAFE(HANDLER) clu_handler_is_safe(HANDLER, CLU_DEFAULT_TAG);
#define CLU_HANDLER_REGISTER(HANDLER) clu_handler_register(HANDLER, CLU_DEFAULT_TAG);
#define CLU_HANDLER_UNREGISTER(HANDLER) clu_handler_unregister(HANDLER, CLU_DEFAULT_TAG);
#define CLU_HANDLER_REGISTER_STATIC(HANDLER) clu_handler_register_static(HANDLER, CLU_DEFAULT_TAG);

#else // DEBUG

#define CLU_HANDLER_IS_SAFE(HANDLER)
#define CLU_HANDLER_REGISTER(HANDLER)
#define CLU_HANDLER_UNREGISTER(HANDLER)
#define CLU_HANDLER_REGISTER_STATIC(HANDLER)

#endif // DEBUG

#endif
