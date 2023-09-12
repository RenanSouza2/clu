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

list_head_p lh_root;

handler_p mem_handler_alloc(size_t size, char const format[], ...)
{
    handler_p h = malloc(size);
    assert(h);

    va_list args;
    va_start(args, format);

    char tag[50];
    vsnprintf(tag, 50, format, args);

    lh_root = mem_list_insert(lh_root, h, tag);
    return h;
}

void mem_handler_free(handler_p h)
{
    mem_list_remove(LH(&lh_root), h);
    free(h);
}

void mem_report()
{
    mem_list_report(lh_root);
}

bool mem_empty()
{
    if(lh_root == NULL) return true;

    mem_report();
    return false;
}
