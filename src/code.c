#include <stdlib.h>

#include "../utils/assert.h"

#include "../bin/header.h"
#include "../lib/list/head/header.h"
#include "../lib/tag/struct.h"

list_head_p lh_root_allocated = NULL;
list_head_p lh_root_freed = NULL;

#undef malloc
#undef calloc
#undef realloc
#undef free



void clu_handler_register(handler_p h, char format[], va_list args)
{
    assert(h);
    
    tag_t tag = clu_tag_format_variadic(format, args);
    clu_list_head_insert(&lh_root_allocated, h, &tag);
    clu_list_head_remove(&lh_root_freed, h);
}

bool clu_handler_free_variadic(handler_p h, char format[], va_list args)
{
    assert(h);

    clu_list_head_remove(&lh_root_allocated, h);

    tag_t tag_free = clu_tag_format("free");
    if(!clu_list_head_insert(&lh_root_freed, h, &tag_free))
    {    
        tag_t tag = clu_tag_format_variadic(format, args);
        printf("\n");
        printf("\ndouble free: %s\t", tag.str);
        printf("\n");
        printf("\n\t");
        return false;
    }
    
    return true;
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

handler_p clu_handler_realloc(handler_p h_old, size_t size, char format[], ...)
{
    assert(size);

    handler_p h = realloc(h_old, size);
    assert(h);

    if(h == h_old) return h;

    va_list args;
    va_start(args, format);
    if(h_old) clu_handler_free_variadic(h_old, format, args);
    clu_handler_register(h, format, args);

    return h;
}

bool clu_handler_free(handler_p h, char format[], ...)
{
    va_list args;
    va_start(args, format);
    if(!clu_handler_free_variadic(h, format, args))
        return false;

    free(h);
    return true;
}



void clu_mem_report(char tag[])
{
    printf("\n----------------------");
    clu_list_report(lh_root_allocated, tag);
    printf("\n----------------------");
}

void clu_mem_report_full(char tag[])
{
    printf("\n----------------------");
    clu_list_report_full(lh_root_allocated, tag);
    printf("\n----------------------");
}

bool clu_mem_empty()
{
    if(lh_root_allocated == NULL) 
    {
        clu_list_head_free(&lh_root_freed);
        return true;
    }

    clu_mem_report("ASSERT");
    return false;
}

handler_p clu_mem_get_pointer(int x, int y)
{
    return clu_list_get_pointer(lh_root_allocated, x, y);
}
