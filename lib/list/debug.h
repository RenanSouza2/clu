#ifndef __LIST_DEBUG_H__
#define __LIST_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

#include <stdbool.h>

#define LH(POINTER) ((list_head_p)(POINTER))
#define HD(POINTER) ((handler_p)(POINTER))

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

void mem_list_body_free(list_body_p lb);
void mem_list_head_free(list_head_p lh);

bool mem_list_body(list_body_p lb, ...);
bool mem_list_head(list_head_p lh, ...);

#else

#define INC(NAME)
#define DEC(NAME)
#define free(POINTER, NAME) free(POINTER)

#endif

#define LB(POINTER) ((list_body_p)(POINTER))

tag_t tag_convert(char const tag_s[]);
bool tag_eq(tag_p tag1, tag_p tag2);

list_body_p mem_list_body_create(handler_p h, list_body_p lb_next);
list_body_p mem_list_body_pop(list_body_p lb);
bool mem_list_body_remove(list_body_p *lb, handler_p h);

list_head_p mem_list_head_create(tag_p tag, handler_p h);
list_head_p mem_list_head_pop(list_head_p lh);
list_head_p mem_list_head_find(list_head_p lh, tag_p tag);

#endif
