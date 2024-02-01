#include <stdlib.h>
#include <assert.h>

#include "header.h"
#include "../lib/list_head/header.h"
#include "../lib/tag/struct.h"

list_head_p lh_root_allocated = NULL;
list_head_p lh_root_freed = NULL;

#undef malloc
#undef calloc
#undef free

void clu_handler_register(handler_p h, char format[], va_list vargs)
{
    assert(h);
    clu_list_head_insert(&lh_root_allocated, h, format, vargs);
    clu_list_head_remove(&lh_root_freed, h, NULL);
}

handler_p clu_handler_malloc(size_t size, char format[], ...)
{
    handler_p h = malloc(size);

    va_list args;
    va_start(args, format);
    clu_handler_register(h, format, args);
    
    return h;
}

handler_p clu_handler_calloc(size_t amt, size_t size, char format[], ...)
{
    handler_p h = calloc(amt, size);

    va_list args;
    va_start(args, format);
    clu_handler_register(h, format, args);
    
    return h;
}

bool clu_handler_free(handler_p h, char format[], ...)
{
    va_list args;
    va_start(args, format);

    tag_t tag_alloc = clu_tag_convert("not registered");
    clu_list_head_remove(&lh_root_allocated, h, &tag_alloc);
    if(!clu_list_head_insert(&lh_root_freed, h, "free", args))
    {
        tag_t tag_free = clu_tag_convert_variadic(format, args);
        printf("\ndouble free: %s\t", tag_free.str);
        printf("\n");
        printf("\n\t");
        return false;
    }
    
    free(h);
    return true;
}



void mem_report(char tag[])
{
    clu_list_report(lh_root_allocated, tag);
}

void mem_report_full(char tag[])
{
    clu_list_report_full(lh_root_allocated, tag);
}

bool mem_empty()
{
    if(lh_root_allocated == NULL) 
    {
        clu_list_head_free(&lh_root_freed);
        return true;
    }

    mem_report("ASSERT");
    return false;
}

handler_p mem_get_pointer(int x, int y)
{
    return clu_list_get_pointer(lh_root_allocated, x, y);
}
