#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

#include "debug.h"
#include "../list/struct.h"

#ifdef DEBUG
#endif

#undef malloc
#undef free

list_head_p lh_root_allocated = NULL;
list_head_p lh_root_freed = NULL;

handler_p mem_handler_alloc(size_t size, char format[], ...)
{
    handler_p h = malloc(size);
    assert(h);

    va_list args;
    va_start(args, format);
    mem_list_head_insert(&lh_root_allocated, h, format, args);
    mem_list_head_remove(&lh_root_freed, h, NULL);
    
    return h;
}

void mem_handler_free(handler_p h, ...)
{
    va_list args;
    va_start(args, h);

    tag_t tag = tag_convert("avulse", NULL);
    mem_list_head_remove(&lh_root_allocated, h, &tag);
    if(!mem_list_head_insert(&lh_root_freed, h, "free", args))
    {
        printf("\ndouble free (%s): %p\t", tag.str, h);
        assert(false);
    }
    free(h);
}



void mem_report(char tag[])
{
    mem_list_report(lh_root_allocated, tag);
}

void mem_report_full(char tag[])
{
    mem_list_report_full(lh_root_allocated, tag);
}

bool mem_empty()
{
    if(lh_root_allocated == NULL) return true;

    mem_report("ASSERT");
    return false;
}

handler_p mem_get_pointer(int x, int y)
{
    return mem_list_get_pointer(lh_root_allocated, x, y);
}