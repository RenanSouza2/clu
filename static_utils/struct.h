#ifndef STRUCT

#define STRUCT(NAME)                            \
    typedef struct _##NAME NAME##_t;            \
    typedef struct _##NAME * NAME##_p;          \
    typedef struct _##NAME const * NAME##_c;    \
    struct _##NAME

#define PLACEHOLDER(NAME)                   \
    typedef struct _##NAME * NAME##_p;      \
    typedef struct _##NAME const * NAME##_c \

typedef void * handler_p;

typedef void const * handler_c;

#endif
