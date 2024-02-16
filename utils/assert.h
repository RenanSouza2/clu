#ifndef __ASSERT_H__
#define __ASSERT_H__

#include <stdio.h>
#include <assert.h>

#undef  assert
#define assert(COND)        \
    if(!(COND))             \
    {                       \
        printf("\n\n");     \
        __assert_fail(#COND, __FILE__, __LINE__, __ASSERT_FUNCTION);    \
    }

#endif
