#include <stdlib.h>

#include "../utils/assert.h"

#include "../bin/header.h"
#include "../lib/list/head/header.h"
#include "../lib/tag/struct.h"

list_head_p lh_root_allocated = NULL;
list_head_p lh_root_freed = NULL;
bool log_allocations = false;

#undef malloc
#undef calloc
#undef realloc
#undef free



void clu_handler_allocate(handler_p h, char format[], va_list args, size_t size, char fn[])
{
    tag_t tag = clu_tag_format_variadic(format, args);

    if(size == 0)
    {
        printf("\n");
        printf("\n\tallocation failure");
        printf("\n\tsize is ZERO");
        printf("\n\ttag: %s", tag.str);
        printf("\n");
        printf("\n\t");
        assert(false);
    }

    if(!h)
    {
        printf("\n");
        printf("\n\tallocation failure");
        printf("\n\tsize: %lu", size);
        printf("\n\tfn: %s", fn);
        printf("\n\ttag: %s", tag.str);
        printf("\n");
        printf("\n\t");
        assert(false);
    }

    assert(clu_list_head_insert(&lh_root_allocated, h, &tag));
    clu_list_head_remove(&lh_root_freed, h);
    if(log_allocations) printf("\n%s | %s: %p\t", fn, tag.str, h);
}

bool clu_handler_deallocate(handler_p h, char format[], va_list args)
{
    assert(h);

    if(!clu_list_head_remove(&lh_root_allocated, h))
    {
        tag_t tag = clu_tag_format_variadic(format, args);
        printf("\n");
        printf("\nfree not allocated pointer: %s\t", tag.str);
        printf("\n");
        printf("\n\t");
        return false;
    }

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

    if(log_allocations) printf("\n\tfree: %p\t", h);
    return true;
}



handler_p clu_handler_malloc(size_t size, char format[], ...)
{
    handler_p h = malloc(size);

    va_list args;
    va_start(args, format);
    clu_handler_allocate(h, format, args, size, "malloc");

    return h;
}

handler_p clu_handler_calloc(size_t amt, size_t size, char format[], ...)
{
    handler_p h = calloc(amt, size);

    va_list args;
    va_start(args, format);
    clu_handler_allocate(h, format, args, size, "calloc");

    return h;
}

handler_p clu_handler_realloc(handler_p volatile h_old, size_t size, char format[], ...)
{
    assert(size);

    handler_p h = realloc(h_old, size);
    assert(h);

    if(h == h_old) return h;

    va_list args;
    va_start(args, format);
    if(h_old) clu_handler_deallocate(h_old, format, args);
    clu_handler_allocate(h, format, args, size, "realloc");

    return h;
}

void clu_handler_free(handler_p h, char format[], ...)
{
    va_list args;
    va_start(args, format);
    assert(clu_handler_deallocate(h, format, args));
    free(h);
}



void _clu_mem_report(char tag[], bool full)
{
    printf("\n----------------------");
    clu_list_report(lh_root_allocated, tag, full);
    printf("\n----------------------");
}

void clu_mem_report(char tag[])
{
    _clu_mem_report(tag, false);
}

void clu_mem_report_full(char tag[])
{
    _clu_mem_report(tag, true);
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

int clu_mem_count(int x)
{
    return clu_list_count(lh_root_allocated, x);
}

handler_p clu_mem_get_pointer(int x, int y)
{
    return clu_list_get_pointer(lh_root_allocated, x, y);
}



bool clu_is_allocated(handler_p h)
{
    return clu_list_head_contains(lh_root_allocated, h);
}

bool clu_is_safe(handler_p h)
{
    return !h || clu_is_allocated(h);
}

bool clu_is_freed(handler_p h)
{
    return clu_list_head_contains(lh_root_freed, h);
}



void clu_set_log(bool _log_allocations)
{
    log_allocations = _log_allocations;
}
