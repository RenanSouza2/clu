#ifndef MEM_DEBUG_H
#define MEM_DEBUG_H

#ifdef DEBUG

#include "../../mods/macros/uint.h" // IWYU pragma: keep

extern uint64_t list_alive;
extern uint64_t trie_alive;

void clu_mem_internal_display();
bool clu_mem_internal_empty();

#define CLU_MEM_INTERNAL_LOG_STATUS false

#define CLU_MEM_INTERNAL_LOG(...)               \
    {                                           \
        if(CLU_MEM_INTERNAL_LOG_STATUS)         \
        {                                       \
            fprintf(stderr, "\nclu internal log\t| ");   \
            fprintf(stderr, __VA_ARGS__);                \
            fprintf(stderr, "\t\t");                     \
        }                                       \
    }

#define INC(HANDLER, NAME)                                  \
    {                                                       \
        CLU_MEM_INTERNAL_LOG("allocating: %p", (handler_p)(HANDLER));   \
        NAME##_alive++;                                     \
    }

#define DEC(HANDLER, NAME)                              \
    {                                                   \
        assert(NAME##_alive);                           \
        CLU_MEM_INTERNAL_LOG("\tfreeing: %p", (handler_p)(HANDLER));    \
        NAME##_alive--;                                 \
    }

#define CALLOC(VAR, NAME)                       \
    {                                           \
        (VAR) = calloc(1, sizeof(NAME##_t));    \
        assert(VAR);                            \
        INC(VAR, NAME);                         \
    }

#define FREE(HANDLER, NAME) \
    {                       \
        DEC(HANDLER, NAME); \
        free(HANDLER);      \
    }

#else // ifdef DEBUG

#define FREE(HANDLER, NAME) free(HANDLER)

#define CALLOC(VAR, NAME)                   \
    {                                       \
        VAR = calloc(1, sizeof(NAME##_t));  \
        assert(VAR);                        \
    }

#endif // ifdef DEBUG

#endif // ifndef MEM_DEBUG_H
