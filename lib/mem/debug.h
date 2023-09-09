#ifndef __MEM_DEBUG_H__
#define __MEM_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

#include <stdbool.h>

extern int list_head_count;
extern int list_body_count;

bool list_memory();

#define INC(NAME) NAME##_count++;
#define DEC(NAME)               \
    {                           \
        assert(NAME##_count);   \
        NAME##_count--;         \
    }

#define free(POINTER, NAME) \
    {                       \
        DEC(NAME);          \
        free(POINTER);      \
    }

#else

#define INC(NAME)
#define DEC(NAME)
#define free(POINTER, NAME) free(POINTER)

#endif

list_body_p list_body_create(handler_p h, list_body_p lb_next);
list_head_p list_head_create(string_t str, list_head_p lh_next);
list_head_p list_head_find(list_head_p lh, string_t str);

#endif
