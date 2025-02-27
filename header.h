#ifndef __ROOT_H__
#define __ROOT_H__

#ifdef DEBUG

#include <stdio.h>
#include <stdbool.h>

#include "./utils/assert.h"

typedef void * handler_p;

handler_p clu_handler_malloc(size_t size, char format[], ...);
handler_p clu_handler_calloc(size_t amt, size_t size, char format[], ...);
handler_p clu_handler_realloc(handler_p h_old, size_t size, char format[], ...);
void clu_handler_free(handler_p h, char format[], ...);

void clu_mem_report(char tag[]);
void clu_mem_report_full(char tag[]);

bool clu_mem_empty();
int clu_mem_count(int x);
handler_p clu_mem_get_pointer(int x, int y);
bool clu_is_allocated(handler_p h);
bool clu_is_safe(handler_p h);
bool clu_is_freed(handler_p h);

void clu_set_log(bool _log_allocations);

#define malloc(SIZE) clu_handler_malloc(SIZE, "f|%s|l|%d", __func__, __LINE__)
#define calloc(AMT, SIZE) clu_handler_calloc(AMT, SIZE, "f|%s|l|%d", __func__, __LINE__)
#define realloc(PTR, SIZE) clu_handler_realloc(PTR, SIZE, "f|%s|l|%d", __func__, __LINE__)
#define free(HANDLER) clu_handler_free(HANDLER, "f|%s|l|%d", __func__, __LINE__)

#define DBG_CHECK_PTR(H) assert(clu_is_safe(H));

#else

#define DBG_CHECK_PTR(H)

#endif // DEBUG

#endif
