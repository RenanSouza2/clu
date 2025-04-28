#include <stdlib.h>

#include "../mods/macros/assert.h"

#include "../header.h"
#include "../lib/list/header.h"
#include "../lib/tag/struct.h"
#include "../lib/trie/header.h"

list_p l_root_allocated = NULL;
trie_p t_root_freed = NULL;
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
        printf("\n");
        printf("\n\ttag : %s", tag.str);
        printf("\n\tfn  : %s", fn);
        printf("\n");
        printf("\n\t");
        assert(false);
    }

    if(h == NULL)
    {
        printf("\n");
        printf("\n");
        printf("\n\t---------------");
        printf("\n\tallocation failure");
        printf("\n\tattempt to register NULL");
        printf("\n");
        printf("\n\tsize : %lu", size);
        printf("\n\ttag  : %s", tag.str);
        printf("\n\tfn   : %s", fn);
        printf("\n");
        printf("\n\t");
        assert(false);
    }

    if(t_root_freed)
        clu_trie_remove(&t_root_freed, h);

    if(!clu_list_insert(&l_root_allocated, &tag, h))
    {
        printf("\n");
        printf("\n");
        printf("\n\t---------------");
        printf("\n\tallocation failure");
        printf("\n\thandler alredy registered");
        printf("\n");
        printf("\n\th    : %p", h);
        printf("\n\tsize : %lu", size);
        printf("\n\ttag  : %s", tag.str);
        printf("\n\tfn   : %s", fn);
        printf("\n");
        printf("\n\t");
        assert(false);
    }

    if(log_allocations) printf("\n%s | %p | %s | %lu\t", fn, h, tag.str, size);
}

void clu_handler_deallocate(handler_p h, char format[], va_list args, char fn[])
{
    tag_t tag = clu_tag_format_variadic(format, args);

    if(h == NULL)
    {
        printf("\n");
        printf("\n");
        printf("\n\t---------------");
        printf("\n\tfree faillure");
        printf("\n");
        printf("\n\tfree NULL pointer\t");
        printf("\n\ttag : %s", tag.str);
        printf("\n\tfn  : %s", fn);
        printf("\n");
        assert(false);
    }

    if(!clu_trie_insert(&t_root_freed, h))
    {
        printf("\n");
        printf("\n");
        printf("\n\t---------------");
        printf("\n\tfree faillure");
        printf("\n\tdouble free");
        printf("\n");
        printf("\n\th   : %p", h);
        printf("\n\ttag : %s", tag.str);
        printf("\n\tfn  : %s", fn);
        printf("\n");
        assert(false);
    }

    if(l_root_allocated == NULL || !clu_list_remove(&l_root_allocated, h))
    {
        printf("\n");
        printf("\n");
        printf("\n\t---------------");
        printf("\n\tfree faillure");
        printf("\n\tfree not allocated pointer: %s\t", tag.str);
        printf("\n");
        printf("\n\th   : %p", h);
        printf("\n\ttag : %s", tag.str);
        printf("\n\tfn  : %s", fn);
        printf("\n");
        assert(false);
    }

    if(log_allocations) printf("\n\t%s | %p | %s\t", fn, h, tag.str);
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
    va_list args;
    va_start(args, format);
    if(h) clu_handler_deallocate(h, format, args, "realloc");

    h = realloc(h, size);
    clu_handler_allocate(h, format, args, size, "realloc");

    return h;
}

void clu_handler_free(handler_p h, char format[], ...)
{
    va_list args;
    va_start(args, format);
    clu_handler_deallocate(h, format, args, "free");
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
    clu_handler_deallocate(h, format, args, "custom");
}



void clu_handler_is_safe(handler_p h, char format[], ...)
{
    if(h == NULL)
        return;

    if(clu_handler_is_freed(h))
    {
        va_list args;
        va_start(args, format);
        tag_t tag = clu_tag_format_variadic(format, args);
        printf("\n");
        printf("\n");
        printf("\n\t---------------");
        printf("\n\thandler not safe");
        printf("\n\thandler already freed");
        printf("\n");
        printf("\n\th   : %p", h);
        printf("\n\ttag : %s", tag.str);
        printf("\n\t");
        assert(false);
    }

    if(!clu_handler_is_allocated(h))
    {
        va_list args;
        va_start(args, format);
        tag_t tag = clu_tag_format_variadic(format, args);
        printf("\n");
        printf("\n");
        printf("\n\t---------------");
        printf("\n\thandler not safe");
        printf("\n\thandler not allocated");
        printf("\n");
        printf("\n\th   : %p", h);
        printf("\n\ttag : %s", tag.str);
        printf("\n\t");
        assert(false);
    }
}



void clu_mem_report_opts(char tag[], bool full)
{
    printf("\n");
    printf("\n----------------------");
    clu_list_report(l_root_allocated, tag, full);
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
    if(l_root_allocated)
    {
        clu_mem_report("ASSERT FAIL | MEMORY NOT EMPTY");
        return false;
    }

    clu_trie_free(t_root_freed);
    t_root_freed = NULL;
    return true;
}

bool clu_handler_is_allocated(handler_p h)
{
    return clu_list_contains(l_root_allocated, h);
}

bool clu_handler_is_freed(handler_p h)
{
    if(t_root_freed)
        return clu_trie_contains(t_root_freed, h);

    return false;
}



uint64_t clu_get_max_i()
{
    return clu_list_count(l_root_allocated);
}

uint64_t clu_get_max_j(uint64_t i)
{
    trie_p t = clu_list_get_trie(l_root_allocated, i);
    return t ? clu_trie_count(t) : 0;
}

handler_p clu_get_handler(uint64_t i, uint64_t j)
{
    trie_p t = clu_list_get_trie(l_root_allocated, i);
    return t ? clu_trie_get_handler(t, j) : NULL;
}



void clu_set_log(bool _log_allocations)
{
    log_allocations = _log_allocations;
}
