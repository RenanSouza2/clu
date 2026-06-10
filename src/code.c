#include <stdlib.h>

#include "../mods/macros/threads.h"

#include "../header.h"
#include "../lib/list/header.h"
#include "../lib/tag/struct.h"
#include "../lib/trie/header.h"



#undef malloc
#undef calloc
#undef realloc
#undef free



static pthread_mutex_t clu_mut;
static pthread_once_t clu_mut_once = PTHREAD_ONCE_INIT;

static list_p clu_l_root_allocated = nullptr;
static list_p clu_l_root_static = nullptr;
static trie_p clu_t_root_freed = nullptr;
static uint64_t clu_log_level = CLU_LOG_DISABLED;
static uint64_t clu_max_occupancy = 0;
static uint64_t clu_occupancy = 0;
static uint64_t clu_register_count = 0;

static void clu_mut_init()
{
    pthread_mutexattr_t attr;
    TREAT(pthread_mutexattr_init(&attr));
    TREAT(pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE));
    TREAT(pthread_mutex_init(&clu_mut, &attr));
    TREAT(pthread_mutexattr_destroy(&attr));
}

static void clu_mut_nested_lock()
{
    TREAT(pthread_once(&clu_mut_once, clu_mut_init));
    TREAT(pthread_mutex_lock(&clu_mut));
}

static void clu_mut_nested_unlock()
{
    TREAT(pthread_mutex_unlock(&clu_mut));
}



static void clu_handler_allocate(
    handler_p h,
    tag_t tag,
    size_t size,
    bool can_be_zero,
    const char fn[]
)
{
    clu_mut_nested_lock();

    if((!can_be_zero) && (size == 0))
    {
        fprintf(stderr, "\n");
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\t---------------");
        fprintf(stderr, "\n\tallocation failure");
        fprintf(stderr, "\n\tsize is ZERO");
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\ttag : %s", tag.str);
        fprintf(stderr, "\n\tfn  : %s", fn);
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\t");
        assert(false);
    }

    if(h == nullptr)
    {
        fprintf(stderr, "\n");
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\t---------------");
        fprintf(stderr, "\n\tallocation failure");
        fprintf(stderr, "\n\tattempt to register nullptr");
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\tsize : %lu", size);
        fprintf(stderr, "\n\ttag  : %s", tag.str);
        fprintf(stderr, "\n\tfn   : %s", fn);
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\t");
        assert(false);
    }

    if(clu_list_contains(clu_l_root_static, h))
    {
        tag_t tag_prev = clu_list_get_tag(clu_l_root_static, h);

        fprintf(stderr, "\n");
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\t---------------");
        fprintf(stderr, "\n\tallocation failure");
        fprintf(stderr, "\n\thandler alredy registered as static");
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\th    : %p", h);
        fprintf(stderr, "\n\tsize : %lu", size);
        fprintf(stderr, "\n\ttag  : %s", tag.str);
        fprintf(stderr, "\n\tfn   : %s", fn);
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\tpreviously registered at: %s", tag_prev.str);
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\t");
        assert(false);
    }

    if(clu_t_root_freed)
    {
        clu_trie_remove(&clu_t_root_freed, h, nullptr);
    }

    if(!clu_list_insert(&clu_l_root_allocated, &tag, h, size))
    {
        tag_t tag_prev = clu_list_get_tag(clu_l_root_allocated, h);

        fprintf(stderr, "\n");
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\t---------------");
        fprintf(stderr, "\n\tallocation failure");
        fprintf(stderr, "\n\thandler alredy registered");
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\th    : %p", h);
        fprintf(stderr, "\n\tsize : %lu", size);
        fprintf(stderr, "\n\ttag  : %s", tag.str);
        fprintf(stderr, "\n\tfn   : %s", fn);
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\tpreviously registered at: %s", tag_prev.str);
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\t");
        assert(false);
    }

    if(clu_log_level >= CLU_LOG_DYNAMIC)
    {
        fprintf(stderr, "\n\t| INCLUDE | %s | %s | %p | %lu\t", fn, tag.str, h, size);
    }

    clu_occupancy += size;
    if(clu_occupancy > clu_max_occupancy)
    {
        clu_max_occupancy = clu_occupancy;
    }

    clu_register_count++;

    clu_mut_nested_unlock();
}

static void clu_handler_deallocate(handler_p h, tag_t tag, const char fn[])
{
    clu_mut_nested_lock();

    if(h == nullptr)
    {
        fprintf(stderr, "\n");
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\t---------------");
        fprintf(stderr, "\n\tfree faillure");
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\tfree nullptr pointer\t");
        fprintf(stderr, "\n\ttag : %s", tag.str);
        fprintf(stderr, "\n\tfn  : %s", fn);
        fprintf(stderr, "\n");
        assert(false);
    }

    if(clu_list_contains(clu_l_root_static, h))
    {
        tag_t tag_prev = clu_list_get_tag(clu_l_root_static, h);

        fprintf(stderr, "\n");
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\t---------------");
        fprintf(stderr, "\n\tfree faillure");
        fprintf(stderr, "\n\tfree static pointer");
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\th    : %p", h);
        fprintf(stderr, "\n\ttag  : %s", tag.str);
        fprintf(stderr, "\n\tfn   : %s", fn);
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\tpreviously registered at: %s", tag_prev.str);
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\t");
        assert(false);
    }

    if(!clu_trie_insert(&clu_t_root_freed, h, 0))
    {
        fprintf(stderr, "\n");
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\t---------------");
        fprintf(stderr, "\n\tfree faillure");
        fprintf(stderr, "\n\tdouble free");
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\th   : %p", h);
        fprintf(stderr, "\n\ttag : %s", tag.str);
        fprintf(stderr, "\n\tfn  : %s", fn);
        fprintf(stderr, "\n");
        assert(false);
    }

    uint64_t size;
    if(clu_l_root_allocated == nullptr || !clu_list_remove(&clu_l_root_allocated, h, &size))
    {
        fprintf(stderr, "\n");
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\t---------------");
        fprintf(stderr, "\n\tfree faillure");
        fprintf(stderr, "\n\tfree not allocated pointer: %s\t", tag.str);
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\th   : %p", h);
        fprintf(stderr, "\n\ttag : %s", tag.str);
        fprintf(stderr, "\n\tfn  : %s", fn);
        fprintf(stderr, "\n");
        assert(false);
    }

    if(clu_log_level >= CLU_LOG_DYNAMIC)
    {
        fprintf(stderr, "\n\t|\t| REMOVE | %s | %s | %p\t", fn, tag.str, h);
    }

    clu_occupancy -= size;

    clu_mut_nested_unlock();
}



handler_p clu_handler_malloc(size_t size, char const format[], ...)
{
    clu_mut_nested_lock();

    handler_p h = malloc(size);

    va_list args;
    va_start(args, format);
    tag_t tag = clu_tag_format_variadic(format, args);
    clu_handler_allocate(h, tag, size, false, "malloc");

    clu_mut_nested_unlock();
    return h;
}

handler_p clu_handler_calloc(size_t amt, size_t size, char const format[], ...)
{
    clu_mut_nested_lock();

    handler_p h = calloc(amt, size);

    va_list args;
    va_start(args, format);
    tag_t tag = clu_tag_format_variadic(format, args);
    clu_handler_allocate(h, tag, amt * size, false, "calloc");

    clu_mut_nested_unlock();
    return h;
}

handler_p clu_handler_realloc(handler_p h, size_t size, char const format[], ...)
{
    clu_mut_nested_lock();

    va_list args;
    va_start(args, format);
    tag_t tag = clu_tag_format_variadic(format, args);
    if(h)
    {
        clu_handler_deallocate(h, tag, "realloc");
    }

    handler_p h_new = realloc(h, size);
    clu_handler_allocate(h_new, tag, size, false, "realloc");

    clu_mut_nested_unlock();
    return h_new;
}

void clu_handler_free(handler_p h, char const format[], ...)
{
    clu_mut_nested_lock();

    va_list args;
    va_start(args, format);
    tag_t tag = clu_tag_format_variadic(format, args);
    clu_handler_deallocate(h, tag, "free");
    free(h);

    clu_mut_nested_unlock();
}



void clu_handler_register(handler_p h, char const format[], ...)
{
    clu_mut_nested_lock();

    va_list args;
    va_start(args, format);
    tag_t tag = clu_tag_format_variadic(format, args);
    clu_handler_allocate(h, tag, 0, true, "custom");

    clu_mut_nested_unlock();
}

void clu_handler_unregister(handler_p h, char const format[], ...)
{
    clu_mut_nested_lock();

    va_list args;
    va_start(args, format);
    tag_t tag = clu_tag_format_variadic(format, args);
    clu_handler_deallocate(h, tag, "custom");

    clu_mut_nested_unlock();
}

void clu_handler_register_static(handler_p h, char const format[], ...)
{
    clu_mut_nested_lock();

    va_list args;
    va_start(args, format);
    tag_t tag = clu_tag_format_variadic(format, args);

    if(h == nullptr)
    {
        fprintf(stderr, "\n");
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\t---------------");
        fprintf(stderr, "\n\tstatic register fallure");
        fprintf(stderr, "\n\tattempt to register nullptr");
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\ttag  : %s", tag.str);
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\t");
        assert(false);
    }

    clu_list_remove(&clu_l_root_static, h, nullptr);
    clu_list_insert(&clu_l_root_static, &tag, h, 0);

    if(clu_log_level >= CLU_LOG_ALL)
    {
        fprintf(stderr, "\n\tstatic | %s | %p\t", tag.str, h);
    }

    clu_mut_nested_unlock();
}



void clu_handler_is_safe(handler_p h, char const format[], ...)
{
    clu_mut_nested_lock();

    if(h == nullptr)
    {
        clu_mut_nested_unlock();
        return;
    }

    if(clu_list_contains(clu_l_root_static, h))
    {
        clu_mut_nested_unlock();
        return;
    }

    if(clu_handler_is_freed(h))
    {
        va_list args;
        va_start(args, format);
        tag_t tag = clu_tag_format_variadic(format, args);
        fprintf(stderr, "\n");
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\t---------------");
        fprintf(stderr, "\n\thandler not safe");
        fprintf(stderr, "\n\thandler already freed");
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\th   : %p", h);
        fprintf(stderr, "\n\ttag : %s", tag.str);
        fprintf(stderr, "\n\t");
        assert(false);
    }

    if(!clu_handler_is_allocated(h))
    {
        va_list args;
        va_start(args, format);
        tag_t tag = clu_tag_format_variadic(format, args);
        fprintf(stderr, "\n");
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\t---------------");
        fprintf(stderr, "\n\thandler not safe");
        fprintf(stderr, "\n\thandler not allocated");
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\th   : %p", h);
        fprintf(stderr, "\n\ttag : %s", tag.str);
        fprintf(stderr, "\n\t");
        assert(false);
    }

    clu_mut_nested_unlock();
}



static void clu_mem_report_opts(char const tag[], bool full)
{
    clu_mut_nested_lock();

    fprintf(stderr, "\n");
    fprintf(stderr, "\n----------------------");
    clu_list_report(clu_l_root_allocated, tag, full);
    fprintf(stderr, "\n----------------------");
    fprintf(stderr, "\n");

    clu_mut_nested_unlock();
}

void clu_mem_report(char const tag[])
{
    clu_mut_nested_lock();
    clu_mem_report_opts(tag, false);
    clu_mut_nested_unlock();
}

void clu_mem_report_full(char const tag[])
{
    clu_mut_nested_lock();
    clu_mem_report_opts(tag, true);
    clu_mut_nested_unlock();
}



bool clu_mem_is_empty()
{
    clu_mut_nested_lock();

    if(clu_l_root_allocated)
    {
        clu_mem_report_opts("ASSERT FAIL | MEMORY NOT EMPTY", clu_log_level >= CLU_LOG_DYNAMIC);
        clu_mut_nested_unlock();
        return false;
    }

    clu_list_free(clu_l_root_static);
    clu_trie_free(clu_t_root_freed);
    clu_l_root_static = nullptr;
    clu_t_root_freed = nullptr;

    clu_mut_nested_unlock();
    return true;
}

bool clu_handler_is_allocated(handler_p h)
{
    clu_mut_nested_lock();
    bool res = clu_list_contains(clu_l_root_allocated, h);
    clu_mut_nested_unlock();
    return res;
}

bool clu_handler_is_static(handler_p h)
{
    clu_mut_nested_lock();
    bool res = clu_list_contains(clu_l_root_static, h);
    clu_mut_nested_unlock();
    return res;
}

bool clu_handler_is_freed(handler_p h)
{
    clu_mut_nested_lock();
    bool res = (clu_t_root_freed != nullptr) && clu_trie_contains(clu_t_root_freed, h);
    clu_mut_nested_unlock();
    return res;
}



uint64_t clu_get_max_i()
{
    clu_mut_nested_lock();
    uint64_t res = clu_list_count(clu_l_root_allocated);
    clu_mut_nested_unlock();
    return res;
}

uint64_t clu_get_max_j(uint64_t i)
{
    clu_mut_nested_lock();
    uint64_t max_i = clu_list_count(clu_l_root_allocated);
    if(i >= max_i)
    {
        fprintf(stderr, "\n");
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\t---------------");
        fprintf(stderr, "\n\tget max j failure");
        fprintf(stderr, "\n\tindex i out of bounds");
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\ti     : " U64P() "", i);
        fprintf(stderr, "\n\tmax_i : " U64P() "", max_i);
        fprintf(stderr, "\n");
        assert(false);
    }
    trie_p t = clu_list_get_trie(clu_l_root_allocated, i);
    uint64_t res = clu_trie_count(t);
    clu_mut_nested_unlock();
    return res;
}

handler_p clu_get_handler(uint64_t i, uint64_t j)
{
    clu_mut_nested_lock();
    uint64_t max_i = clu_list_count(clu_l_root_allocated);
    if(i >= max_i)
    {
        fprintf(stderr, "\n");
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\t---------------");
        fprintf(stderr, "\n\tget handler failure");
        fprintf(stderr, "\n\tindex i out of bounds");
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\ti     : " U64P() "", i);
        fprintf(stderr, "\n\tmax_i : " U64P() "", max_i);
        fprintf(stderr, "\n");
        assert(false);
    }
    trie_p t = clu_list_get_trie(clu_l_root_allocated, i);
    uint64_t max_j = clu_trie_count(t);
    if(j >= max_j)
    {
        fprintf(stderr, "\n");
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\t---------------");
        fprintf(stderr, "\n\tget handler failure");
        fprintf(stderr, "\n\tindex j out of bounds");
        fprintf(stderr, "\n");
        fprintf(stderr, "\n\tj     : " U64P() "", j);
        fprintf(stderr, "\n\tmax_j : " U64P() "", max_j);
        fprintf(stderr, "\n");
        assert(false);
    }
    handler_p res = clu_trie_get_handler(t, j);
    clu_mut_nested_unlock();
    return res;
}



void clu_log_level_set(uint64_t _clu_log_level)
{
    clu_mut_nested_lock();
    clu_log_level = _clu_log_level;
    clu_mut_nested_unlock();
}



uint64_t clu_get_occupancy()
{
    clu_mut_nested_lock();
    uint64_t res = clu_occupancy;
    clu_mut_nested_unlock();
    return res;
}

uint64_t clu_get_max_occupancy()
{
    clu_mut_nested_lock();
    uint64_t res = clu_max_occupancy;
    clu_mut_nested_unlock();
    return res;
}

void clu_clean_max_occupancy()
{
    clu_mut_nested_lock();
    clu_max_occupancy = 0;
    clu_mut_nested_unlock();
}

uint64_t clu_get_register_count()
{
    clu_mut_nested_lock();
    uint64_t res = clu_register_count;
    clu_mut_nested_unlock();
    return res;
}
