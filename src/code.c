#include <stdlib.h>

#include "../mods/macros/assert.h"
#include "../mods/macros/threads.h"

#include "../header.h"
#include "../lib/list/header.h"
#include "../lib/tag/struct.h"
#include "../lib/trie/header.h"

list_p l_root_allocated = NULL;
trie_p t_root_freed = NULL;
bool log_allocations = false;


STRUCT(clu_mutex_nested)
{
    pthread_mutex_t mut;
    pthread_t thread_id;
    uint64_t depth;
};

clu_mutex_nested_t clu_mut = (clu_mutex_nested_t)
{
    .mut = PTHREAD_MUTEX_INITIALIZER,
    .thread_id = 0,
    .depth = 0
};

void clu_mut_nested_lock(clu_mutex_nested_p mut)
{
    if(pthread_mutex_trylock(&mut->mut) != 0)
    {
        if(mut->thread_id == pthread_self())
        {
            mut->depth++;
            return;
        }
        
        TREAT(pthread_mutex_lock(&mut->mut));
    }

    assert(mut->thread_id == 0);
    assert(mut->depth == 0);

    mut->thread_id = pthread_self();
    mut->depth++;
}

void clu_mut_nested_unlock(clu_mutex_nested_p mut)
{
    assert(mut->depth);
    mut->depth--;
    if(mut->depth)
        return;

    mut->thread_id = 0;
    TREAT(pthread_mutex_unlock(&mut->mut));
}


#undef malloc
#undef calloc
#undef realloc
#undef free



void clu_handler_allocate(handler_p h, tag_t tag, size_t size, char fn[])
{
    clu_mut_nested_lock(&clu_mut);

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
        exit(EXIT_FAILURE);
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
        exit(EXIT_FAILURE);
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
        exit(EXIT_FAILURE);
    }

    if(log_allocations)
        printf("\n%s | %p | %s | %lu\t", fn, h, tag.str, size);

    clu_mut_nested_unlock(&clu_mut);
}

void clu_handler_deallocate(handler_p h, tag_t tag, char fn[])
{
    clu_mut_nested_lock(&clu_mut);

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
        exit(EXIT_FAILURE);
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
        exit(EXIT_FAILURE);
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
        exit(EXIT_FAILURE);
    }

    if(log_allocations)
        printf("\n\t%s | %p | %s\t", fn, h, tag.str);

    clu_mut_nested_unlock(&clu_mut);
}



handler_p clu_handler_malloc(size_t size, char format[], ...)
{
    clu_mut_nested_lock(&clu_mut);

    handler_p h = malloc(size);

    va_list args;
    va_start(args, format);
    tag_t tag = clu_tag_format_variadic(format, args);
    clu_handler_allocate(h, tag, size, "malloc");

    clu_mut_nested_unlock(&clu_mut);
    return h;
}

handler_p clu_handler_calloc(size_t amt, size_t size, char format[], ...)
{
    clu_mut_nested_lock(&clu_mut);

    handler_p h = calloc(amt, size);

    va_list args;
    va_start(args, format);
    tag_t tag = clu_tag_format_variadic(format, args);
    clu_handler_allocate(h, tag, amt * size, "calloc");

    clu_mut_nested_unlock(&clu_mut);
    return h;
}

handler_p clu_handler_realloc(handler_p h, size_t size, char format[], ...)
{
    clu_mut_nested_lock(&clu_mut);

    va_list args;
    va_start(args, format);
    tag_t tag = clu_tag_format_variadic(format, args);
    if(h)
        clu_handler_deallocate(h, tag, "realloc");

    h = realloc(h, size);
    clu_handler_allocate(h, tag, size, "realloc");

    clu_mut_nested_unlock(&clu_mut);
    return h;
}

void clu_handler_free(handler_p h, char format[], ...)
{
    clu_mut_nested_lock(&clu_mut);

    va_list args;
    va_start(args, format);
    tag_t tag = clu_tag_format_variadic(format, args);
    clu_handler_deallocate(h, tag, "free");
    free(h);

    clu_mut_nested_unlock(&clu_mut);
}



void clu_handler_register(handler_p h, char format[], ...)
{
    clu_mut_nested_lock(&clu_mut);

    va_list args;
    va_start(args, format);
    tag_t tag = clu_tag_format_variadic(format, args);
    clu_handler_allocate(h, tag, 1, "custom");

    clu_mut_nested_unlock(&clu_mut);
}

void clu_handler_unregister(handler_p h, char format[], ...)
{
    clu_mut_nested_lock(&clu_mut);

    va_list args;
    va_start(args, format);
    tag_t tag = clu_tag_format_variadic(format, args);
    clu_handler_deallocate(h, tag, "custom");
    
    clu_mut_nested_unlock(&clu_mut);
}



void clu_handler_is_safe(handler_p h, char format[], ...)
{
    clu_mut_nested_lock(&clu_mut);

    if(h == NULL)
    {
        clu_mut_nested_unlock(&clu_mut);
        return;
    }

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
        exit(EXIT_FAILURE);
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
        exit(EXIT_FAILURE);
    }

    clu_mut_nested_unlock(&clu_mut);
}



void clu_mem_report_opts(char tag[], bool full)
{
    clu_mut_nested_lock(&clu_mut);

    printf("\n");
    printf("\n----------------------");
    clu_list_report(l_root_allocated, tag, full);
    printf("\n----------------------");
    printf("\n");

    clu_mut_nested_unlock(&clu_mut);
}

void clu_mem_report(char tag[])
{
    clu_mut_nested_lock(&clu_mut);
    clu_mem_report_opts(tag, false);
    clu_mut_nested_unlock(&clu_mut);
}

void clu_mem_report_full(char tag[])
{
    clu_mut_nested_lock(&clu_mut);
    clu_mem_report_opts(tag, true);
    clu_mut_nested_unlock(&clu_mut);
}



bool clu_mem_is_empty()
{
    clu_mut_nested_lock(&clu_mut);

    if(l_root_allocated)
    {
        clu_mem_report_opts("ASSERT FAIL | MEMORY NOT EMPTY", false);
        clu_mut_nested_unlock(&clu_mut);
        return false;
    }

    clu_trie_free(t_root_freed);
    t_root_freed = NULL;

    clu_mut_nested_unlock(&clu_mut);
    return true;
}

bool clu_handler_is_allocated(handler_p h)
{
    clu_mut_nested_lock(&clu_mut);
    bool res = clu_list_contains(l_root_allocated, h);
    clu_mut_nested_unlock(&clu_mut);
    return res;
}

bool clu_handler_is_freed(handler_p h)
{
    clu_mut_nested_lock(&clu_mut);
    bool res = t_root_freed ?
        clu_trie_contains(t_root_freed, h) : false;
    clu_mut_nested_unlock(&clu_mut);
    return res;
}



uint64_t clu_get_max_i()
{
    clu_mut_nested_lock(&clu_mut);
    uint64_t res = clu_list_count(l_root_allocated);
    clu_mut_nested_unlock(&clu_mut);
    return res;
}

uint64_t clu_get_max_j(uint64_t i)
{
    clu_mut_nested_lock(&clu_mut);
    trie_p t = clu_list_get_trie(l_root_allocated, i);
    uint64_t res = clu_trie_count(t);
    clu_mut_nested_unlock(&clu_mut);
    return res;
}

handler_p clu_get_handler(uint64_t i, uint64_t j)
{
    clu_mut_nested_lock(&clu_mut);
    trie_p t = clu_list_get_trie(l_root_allocated, i);
    handler_p res = clu_trie_get_handler(t, j);
    clu_mut_nested_unlock(&clu_mut);
    return res;
}



void clu_set_log(bool _log_allocations)
{
    clu_mut_nested_lock(&clu_mut);
    log_allocations = _log_allocations;
    clu_mut_nested_unlock(&clu_mut);
}
