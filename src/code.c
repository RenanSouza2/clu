#include <stdlib.h>

#include "../lib/macros/assert.h"

#include "../header.h"
#include "../lib/list/body/header.h"
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
        printf("\n");
        printf("\n\t---------------");
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
        printf("\n");
        printf("\n\t---------------");
        printf("\n\tallocation failure");
        printf("\n\tsize: %lu", size);
        printf("\n\tfn: %s", fn);
        printf("\n\ttag: %s", tag.str);
        printf("\n");
        printf("\n\t");
        assert(false);
    }

    clu_list_head_remove(&lh_root_freed, h);
    assert(clu_list_head_insert(&lh_root_allocated, &tag, h));

    if(log_allocations) printf("\n%s | %s: %p\t", fn, tag.str, h);
}

void clu_handler_deallocate(handler_p h, char format[], va_list args)
{
    if(h == NULL)
    {
        printf("\n");
        printf("\n");
        printf("\n\t---------------");
        printf("\n\tfree NULL pointer\t");
        printf("\n\t");
        assert(false);
    }

    tag_t tag_free = clu_tag_format("free");
    if(!clu_list_head_insert(&lh_root_freed, &tag_free, h))
    {
        tag_t tag = clu_tag_format_variadic(format, args);
        printf("\n");
        printf("\n");
        printf("\n\t---------------");
        printf("\n\tdouble free: %s\t", tag.str);
        printf("\n\t");
        assert(false);
    }

    if(!clu_list_head_remove(&lh_root_allocated, h))
    {
        tag_t tag = clu_tag_format_variadic(format, args);
        printf("\n");
        printf("\n");
        printf("\n\tfree not allocated pointer: %s\t", tag.str);
        printf("\n\t");
        assert(false);
    }

    if(log_allocations) printf("\n\tfree: %p\t", h);
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

handler_p clu_handler_realloc(handler_p h, size_t size, char format[], ...)
{
    assert(size);

    va_list args;
    va_start(args, format);
    if(h) clu_handler_deallocate(h, format, args);
    h = realloc(h, size);
    assert(h);
    clu_handler_allocate(h, format, args, size, "realloc");
    return h;
}

void clu_handler_free(handler_p h, char format[], ...)
{
    va_list args;
    va_start(args, format);
    clu_handler_deallocate(h, format, args);
    free(h);
}



void clu_handler_register(handler_p h, char format[], ...)
{
    va_list args;
    va_start(args, format);
    clu_handler_allocate(h, format, args, 1, "custom");
}

void clu_handler_unregister(handler_p h, char format[], ...)
{
    va_list args;
    va_start(args, format);
    clu_handler_deallocate(h, format, args);
}



void clu_mem_report_opts(char tag[], bool full)
{
    printf("\n");
    printf("\n----------------------");
    clu_list_head_report(lh_root_allocated, tag, full);
    printf("\n----------------------");
    printf("\n");
}

void clu_mem_report(char tag[])
{
    clu_mem_report_opts(tag, false);
}

void clu_mem_report_full(char tag[])
{
    clu_mem_report_opts(tag, true);
}



bool clu_mem_is_empty()
{
    if(lh_root_allocated)
    {
        clu_mem_report("ASSERT FAIL | MEMORY NOT EMPTY");
        return false;
    }

    lh_root_freed = clu_list_head_free(lh_root_freed);
    return true;
}

bool clu_handler_is_allocated(handler_p h)
{
    return clu_list_head_contains(lh_root_allocated, h);
}

bool clu_handler_is_safe(handler_p h)
{
    return !h || clu_handler_is_allocated(h);
}

bool clu_handler_is_freed(handler_p h)
{
    return clu_list_head_contains(lh_root_freed, h);
}



uint64_t clu_get_max_i()
{
    return clu_list_head_count(lh_root_allocated);
}

uint64_t clu_get_max_j(uint64_t i)
{
    list_body_p lb = clu_list_head_get_body(lh_root_allocated, i);
    return lb ? clu_list_body_count(lb) : 0;
}

handler_p clu_get_handler(uint64_t i, uint64_t j)
{
    list_body_p lb = clu_list_head_get_body(lh_root_allocated, i);
    return lb ? clu_list_body_get_handler(lb, j) : NULL;
}



void clu_set_log(bool _log_allocations)
{
    log_allocations = _log_allocations;
}
