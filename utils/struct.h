#ifndef STRUCT

#define STRUCT(NAME)                    \
    typedef struct _##NAME NAME##_t;    \
    typedef struct _##NAME * NAME##_p;  \
    struct _##NAME

#define PLACEHOLDER(NAME)                   \
    typedef struct _##NAME * NAME##_p;

typedef void * handler_p;

#ifdef DEBUG

#define HD(POINTER) ((handler_p)(POINTER))

#endif

#endif
