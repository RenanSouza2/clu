#ifndef __MEM_DEBUG_H__
#define __MEM_DEBUG_H__

#ifdef DEBUG

#include <stdbool.h>

#include "../../mods/macros/U64.h"

extern uint64_t list_alive;
extern uint64_t trie_alive;

void clu_mem_internal_display();
bool clu_mem_internal_empty();

#define CLU_MEM_INTERNAL_LOG_STATUS false

#define CLU_MEM_INTERNAL_LOG(...)               \
    {                                           \
        if(CLU_MEM_INTERNAL_LOG_STATUS)         \
        {                                       \
            printf("\nclu internal log\t| ");   \
            printf(__VA_ARGS__);                \
            printf("\t\t");                     \
        }                                       \
    }

#define INC(HANDLER, NAME)                                  \
    {                                                       \
        CLU_MEM_INTERNAL_LOG("allocating: %p", HANDLER);    \
        NAME##_alive++;                                     \
    }

#define DEC(HANDLER, NAME)                              \
    {                                                   \
        assert(NAME##_alive);                           \
        CLU_MEM_INTERNAL_LOG("\tfreeing: %p", HANDLER); \
        NAME##_alive--;                                 \
    }

#define CALLOC(VAR, NAME)                   \
    {                                       \
        VAR = calloc(1, sizeof(NAME##_t));  \
        assert(VAR);                        \
        INC(VAR, NAME);                     \
    }

#define FREE(HANDLER, NAME) \
    {                       \
        DEC(HANDLER, NAME); \
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
