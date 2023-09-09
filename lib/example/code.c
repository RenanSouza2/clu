#include <stdio.h>

#include "debug.h"

#ifdef DEBUG

void hello_debug()
{
    printf("\n\nThis function helps to debug");
    printf("\nbut is not compiled in the final build");
    printf("\n");
}

#endif

void hello_example()
{
    printf("\nHello example library");
}
