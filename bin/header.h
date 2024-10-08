#ifndef __ROOT_H__
#define __ROOT_H__

#include <stdio.h>
#include <stdbool.h>

#include "../utils/assert.h"

typedef void * handler_p;

handler_p clu_handler_malloc(size_t size, char format[], ...);
handler_p clu_handler_calloc(size_t amt, size_t size, char format[], ...);
handler_p clu_handler_realloc(handler_p h_old, size_t size, char format[], ...);
void clu_handler_free(handler_p h, char format[], ...);

bool clu_mem_empty();
void clu_mem_report(char tag[]);
void clu_mem_report_full(char tag[]);
handler_p clu_mem_get_pointer(int x, int y);

#define malloc(SIZE) clu_handler_malloc(SIZE, "f|%s|l|%d", __func__, __LINE__)
#define calloc(AMT, SIZE) clu_handler_calloc(AMT, SIZE, "f|%s|l|%d", __func__, __LINE__)
#define realloc(PTR, SIZE) clu_handler_realloc(PTR, SIZE, "f|%s|l|%d", __func__, __LINE__)
#define free(HANDLER) clu_handler_free(HANDLER, "f|%s|l|%d", __func__, __LINE__)

#endif
