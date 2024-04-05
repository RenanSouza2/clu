#ifndef __MEM_DEBUG_H__
#define __MEM_DEBUG_H__

#ifdef DEBUG

#include <stdbool.h>

extern int list_head_alive;
extern int list_body_alive;

bool clu_mem_empty_internal();

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
