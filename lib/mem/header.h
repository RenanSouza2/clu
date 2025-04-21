#ifndef __MEM_DEBUG_H__
#define __MEM_DEBUG_H__

#ifdef DEBUG

#include <stdbool.h>

#include "../../mods/macros/U64.h"

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

#define CALLOC(VAR, NAME)                   \
    {                                       \
        VAR = calloc(1, sizeof(NAME##_t));  \
        assert(VAR);                        \
        INC(NAME);                          \
    }

#define FREE(HANDLER, NAME) \
    {                       \
        DEC(NAME);          \
        free(HANDLER);      \
    }

#else

#define FREE(HANDLER, NAME) free(HANDLER)
#define CALLOC(VAR, NAME)                   \
    {                                       \
        VAR = calloc(1, sizeof(NAME##_t));  \
        assert(VAR);                        \
    }

#endif

#endif
