#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "debug.h"
#include "../list/header.h"

#ifdef DEBUG
#endif

#undef malloc
#undef free

list_head_p lh_root;

handler_p mem_handler_alloc(size_t size, char const func[])
{
    handler_p h = malloc(size);
    assert(h);

    lh_root = mem_list_insert(lh_root, h, func);
    return h;
}

void mem_handler_free(handler_p h)
{
    mem_list_remove(LH(&lh_root), h);
    free(h);
}

void mem_report()
{
    mem_list_report(lh_root);
}

bool mem_empty()
{
    if(lh_root == NULL) return true;

    mem_report();
    return false;
}
