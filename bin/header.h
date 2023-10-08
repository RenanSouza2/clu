#ifndef __ROOT_H__
#define __ROOT_H__

#include <stdio.h>
#include <stdbool.h>

typedef void * handler_p;

handler_p mem_handler_alloc(size_t size, char const format[], ...);
void mem_handler_free(handler_p h);

bool mem_empty();
void mem_report();
void mem_report_full();
handler_p mem_get_pointer(int x, int y);

#define malloc(SIZE) mem_handler_alloc(SIZE, "f|%s|l|%d", __func__, __LINE__)
#define malloc_tag(SIZE, FORMAT, ARGS...) mem_handler_alloc(SIZE, FORMAT, ARGS)
#define free(HANDLER) mem_handler_free(HANDLER)

#endif
