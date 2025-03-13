#ifndef __MEM_DEBUG_H__
#define __MEM_DEBUG_H__

#ifdef DEBUG

#include <stdbool.h>
#include <stdbool.h>

extern uint64_t list_head_alive;
extern uint64_t list_body_alive;

void clu_mem_internal_display();
bool clu_mem_internal_empty();

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

#endif
