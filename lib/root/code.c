#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

#include "debug.h"
#include "../list/header.h"

#ifdef DEBUG
#endif

#undef malloc
#undef free

list_head_p lh_root_inserted = NULL;

handler_p mem_handler_alloc(size_t size, char const format[], ...)
{
    handler_p h = malloc(size);
    assert(h);

    va_list args;
    va_start(args, format);

    char tag[50];
    vsnprintf(tag, 50, format, args);

    printf("\ninserting (%s): %p", tag, h);

    mem_list_head_insert(&lh_root_inserted, h, tag);
    
    return h;
}

void mem_handler_free(handler_p h)
{
    printf("\nremove: %p", h);
    mem_list_head_remove(&lh_root_inserted, h);
    free(h);
}



void mem_report(char tag[])
{
    mem_list_report(lh_root_inserted, tag);
}

void mem_report_full(char tag[])
{
    mem_list_report_full(lh_root_inserted, tag);
}

bool mem_empty()
{
    if(lh_root_inserted == NULL) return true;

    mem_report("ASSERT");
    return false;
}

handler_p mem_get_pointer(int x, int y)
{
    return mem_list_get_pointer(lh_root_inserted, x, y);
}