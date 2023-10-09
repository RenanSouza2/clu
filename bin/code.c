#include <stdlib.h>
#include <assert.h>

#include "header.h"
#include "../lib/list_head/header.h"
#include "../lib/tag/struct.h"

list_head_p lh_root_allocated = NULL;
list_head_p lh_root_freed = NULL;

#undef malloc

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

    tag_t tag = mem_tag_convert("avulse");
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
    if(lh_root_allocated == NULL) 
    {
        mem_list_head_free(&lh_root_freed);
        return true;
    }

    mem_report("ASSERT");
    return false;
}

handler_p mem_get_pointer(int x, int y)
{
    return mem_list_get_pointer(lh_root_allocated, x, y);
}