#ifndef __LIST_DEBUG_H__
#define __LIST_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

#include <stdbool.h>

extern int list_head_alive;
extern int list_body_alive;

bool list_memory();

#define INC(NAME) NAME##_alive++;
#define DEC(NAME)               \
    {                           \
        assert(NAME##_alive);   \
        NAME##_alive--;         \
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

#define LB(POINTER) ((list_body_p)(POINTER))

void string_convert(string_p str, char const str_s[]);

list_body_p mem_list_body_create(handler_p h, list_body_p lb_next);
list_body_p mem_list_body_pop(list_body_p lb);
bool mem_list_body_remove(list_body_p lb, handler_p h);

list_head_p mem_list_head_create(string_p str, list_head_p lh_next);
list_head_p mem_list_head_pop(list_head_p lh);
list_head_p mem_list_head_find(list_head_p lh, string_p str);

#endif
