#ifndef __CLU_H__
#define __CLU_H__

#include <stdlib.h>

#ifdef DEBUG

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

typedef void * handler_p;

handler_p clu_handler_malloc(size_t size, char format[], ...);
handler_p clu_handler_calloc(size_t amt, size_t size, char format[], ...);
handler_p clu_handler_realloc(handler_p h, size_t size, char format[], ...);
void clu_handler_free(handler_p h, char format[], ...);

void clu_handler_register(handler_p h, char format[], ...);
void clu_handler_unregister(handler_p h, char format[], ...);

void clu_handler_is_safe(handler_p h, char format[], ...);

void clu_mem_report(char tag[]);
void clu_mem_report_full(char tag[]);
bool clu_mem_is_empty();

bool clu_handler_is_allocated(handler_p h);
bool clu_handler_is_freed(handler_p h);

uint64_t clu_get_max_i();
uint64_t clu_get_max_j(uint64_t i);
handler_p clu_get_handler(uint64_t i, uint64_t j);

void clu_set_log(bool _log_allocations);

#define CLU_DEFAULT_TAG "f|%s|l|%d", __func__, __LINE__

#define malloc(SIZE) clu_handler_malloc(SIZE, CLU_DEFAULT_TAG)
#define calloc(AMOUNT, SIZE) clu_handler_calloc(AMOUNT, SIZE, CLU_DEFAULT_TAG)
#define realloc(HANDLER, SIZE) clu_handler_realloc(HANDLER, SIZE, CLU_DEFAULT_TAG)
#define free(HANDLER) clu_handler_free(HANDLER, CLU_DEFAULT_TAG)

#define CLU_HANDLER_IS_SAFE(HANDLER) clu_handler_is_safe(HANDLER, CLU_DEFAULT_TAG);
#define CLU_HANDLER_REGISTER(HANDLER) clu_handler_register(HANDLER, CLU_DEFAULT_TAG)
#define CLU_HANDLER_UNREGISTER(HANDLER) clu_handler_unregister(HANDLER, CLU_DEFAULT_TAG)

#else

#define CLU_HANDLER_IS_SAFE(HANDLER)
#define CLU_HANDLER_REGISTER(HANDLER)
#define CLU_HANDLER_UNREGISTER(HANDLER)

#endif // DEBUG

#endif
