#include <stdlib.h>

#include "debug.h"
#include "../../mem/header.h"
#include "../../../mods/macros/assert.h"
#include "../../../mods/macros/U64.h"



#ifdef DEBUG

#include <stdio.h>
#include <stdarg.h>

trie_p clu_trie_create_variadic_tree_rec(va_list *args)
{
    handler_p h = va_arg(*args, handler_p);
    trie_p t = clu_trie_create(h);
    if(h)
        return t;

    uint64_t n = va_arg(*args, uint64_t);
    assert(n > 0);
    assert(n <= SIZE);
    for(uint64_t i=0; i<n; i++)
    {
        uint64_t k = va_arg(*args, uint64_t);
        assert(k < SIZE);
        assert(t->arr[k] == NULL);
        t->arr[k] = clu_trie_create_variadic_tree_rec(args);
    }
    return t;
}

trie_p clu_trie_create_variadic_tree(bool content, va_list *args)
{
    if(!content)
        return NULL;

    return clu_trie_create_variadic_tree_rec(args);
}

trie_p clu_trie_create_immed_tree(int content, ...)
{
    va_list args;
    va_start(args, content);
    return  clu_trie_create_variadic_tree(content, &args);
}



trie_p clu_trie_create_variadic_list(uint64_t n, va_list *args)
{
    assert(n > 0);

    trie_p t = NULL;
    for(uint64_t i=0; i<n; i++)
    {
        handler_p h = va_arg(*args, handler_p);
        assert(clu_trie_insert(&t, h));
    }

    uint64_t n_remove = va_arg(*args, uint64_t);
    assert(n_remove < n);
    for(uint64_t i=0; i<n_remove; i++)
    {
        handler_p h = va_arg(*args, handler_p);
        assert(clu_trie_remove(&t, h));
    }
    return t;
}

trie_p clu_trie_create_immed_list(uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    return clu_trie_create_variadic_list(n, &args);
}



void clu_trie_display_dbg_rec(trie_p t, uint64_t index)
{
    assert(t);

    if(t->h)
    {
        if(index) printf("\t");
        printf("h: %p", t->h);
        return;
    }

    for(uint64_t i=0; i<SIZE; i++)
        if(t->arr[i])
        {
            printf("\n");
            for(uint64_t k=0; k<index; k++)
                printf("\t");
            printf("i: " U64P() "", i);
            clu_trie_display_dbg_rec(t->arr[i], index + 1);
        }
}

void clu_trie_display_dbg(trie_p t)
{
    if(t == NULL)
    {
        printf("\nEMPTY LIST");
        return;
    }

    printf("\n");
    clu_trie_display_dbg_rec(t, 0);
    printf("\n");
}



bool uint64(uint64_t i1, uint64_t i2)
{
    if(i1 != i2)
    {
        printf("\n\n\tUINT64 ASSERT ERROR\t| " U64P() " " U64P() "", i1, i2);
        return false;
    }

    return true;
}

bool clu_trie_rec(trie_p t_1, trie_p t_2, handler_p h, uint64_t index)
{
    if(t_1 == NULL)
    {
        if(t_2 != NULL)
        {
            printf("\n\n\tTRIE ASSERT ERROR\t| L1 EMPTY L2 NOT | H %p | I " U64P() "", h, index);
            return false;
        }

        return true;
    }

    if(t_2 == NULL)
    {
        printf("\n\n\tTRIE ASSERT ERROR\t| L1 NOT EMPTY L2 IS | H %p | I " U64P() "", h, index);
        return false;
    }

    if(t_1->h != t_2->h)
    {
        printf("\n\n\tTRIE ASSERT ERROR\t| H MISMATCH 1 | %p %p | H %p | I " U64P() "", t_1->h, t_2->h, h, index);
        return false;
    }

    if(t_1->h != NULL)
    {
        for(uint64_t i=0; i<index; i++)
            if(!uint64(GET(t_1->h, i), GET(h, i)))
            {
                printf("\n\tTRIE ASSERT ERROR\t| H MISMATCH 2 | %p | H  %p | I " U64P() "", t_1->h, h, index);
                return false;
            }

        for(uint64_t i=0; i<16; i++)
            if(t_1->arr[i])
            {
                printf("\n\n\tTRIE ASSERT ERROR\t| L1 HAS H AND BRANCH | %p " U64P() " " U64P() "", h, i, index);
                return false;
            }

        return true;
    }

    for(uint64_t i=0; i<16; i++)
        if(!clu_trie_rec(t_1->arr[i], t_2->arr[i], SET(h, index, i), index + 1))
            return false;

    return true;
}

bool clu_trie_inner(trie_p t_1, trie_p t_2)
{
    if(!clu_trie_rec(t_1, t_2, NULL, 0))
    {
        clu_trie_display_dbg(t_1);
        clu_trie_display_dbg(t_2);
        return false;
    }

    return true;
}

bool clu_trie(trie_p t_1, trie_p t_2)
{
    if(!clu_trie_inner(t_1, t_2))
        return false;

    clu_trie_free(t_1);
    clu_trie_free(t_2);
    return true;
}

bool clu_trie_immed_tree(trie_p t, int content, ...)
{
    va_list args;
    va_start(args, content);
    trie_p t_2 = clu_trie_create_variadic_tree(content, &args);
    return clu_trie(t, t_2);
}

bool clu_trie_immed_list(trie_p t, uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    trie_p t_2 = clu_trie_create_variadic_list(n, &args);
    return clu_trie(t, t_2);
}

#endif



void clu_trie_display(trie_p t)
{
    if(t == NULL)
        return;

    if(t->h)
    {
        printf("\n\t%p\t", t->h);
        return;
    }

    for(uint64_t i=0; i<SIZE; i++)
        clu_trie_display(t->arr[i]);
}



trie_p clu_trie_create(handler_p h)
{
    trie_p t;
    CALLOC(t, trie);

    t->h = h;
    return t;
}

void clu_trie_free(trie_p t)
{
    if(t == NULL)
        return;

    if(t->h == NULL)
    {
        for(uint64_t i=0; i<SIZE; i++)
            clu_trie_free(t->arr[i]);
    }

    FREE(t, trie);
}



bool clu_trie_insert_rec(trie_p *t_root, handler_p h, uint64_t index)
{
    trie_p t = *t_root;
    uint64_t key = GET(h, index);

    if(t == NULL)
    {
        *t_root = clu_trie_create(h);
        return true;
    }

    if(t->h)
    {
        if(t->h == h)
            return false;

        assert(clu_trie_insert_rec(&t->arr[GET(t->h, index)], t->h, index + 1));
        t->h = NULL;
    }

    return clu_trie_insert_rec(&t->arr[key], h, index + 1);
}

bool clu_trie_remove_rec(trie_p *t_root, handler_p h, uint64_t index)
{
    trie_p t = *t_root;
    if(t == NULL)
        return false;

    if(t->h)
    {
        if(t->h != h)
            return false;

        FREE(t, trie);
        *t_root = NULL;
        return true;
    }

    if(!clu_trie_remove_rec(&t->arr[GET(h, index)], h, index + 1))
        return false;

    for(uint64_t i=0; i<SIZE; i++)
        if(t->arr[i])
            return true;

    FREE(t, trie);
    *t_root = NULL;
    return true;
}



bool clu_trie_insert(trie_p *t_root, handler_p h)
{
    assert(t_root);
    assert(h);

    return clu_trie_insert_rec(t_root, h, 0);
}

bool clu_trie_remove(trie_p *t_root, handler_p h)
{
    assert(t_root);
    assert(*t_root);
    assert(h);

    return clu_trie_remove_rec(t_root, h, 0);
}



handler_p clu_trie_get_handler_rec(trie_p t, uint64_t j, bool revert)
{
    assert(t);

    if(t->h)
    {
        if(j == 0)
            return t->h;

        assert(!revert);
        return NULL;
    }

    for(uint64_t i=0; i<SIZE; i++)
    {
        uint64_t count = clu_trie_count(t->arr[i]);
        if(j < count)
            return clu_trie_get_handler_rec(t->arr[i], j, true);

        j -= count;
    }

    assert(!revert);
    return NULL;
}

bool clu_trie_contains_rec(trie_p t, handler_p h, uint64_t index)
{
    if(t == NULL)
        return false;

    if(t->h)
        return t->h == h;

    return clu_trie_contains_rec(t->arr[GET(h, index)], h, index + 1);
}



uint64_t clu_trie_count(trie_p t)
{
    if(t == NULL)
        return 0;

    if(t->h)
        return 1;

    uint64_t count = 0;
    for(uint64_t i=0; i<SIZE; i++)
        count += clu_trie_count(t->arr[i]);

    return count;
}

handler_p clu_trie_get_handler(trie_p t, uint64_t j)
{
    assert(t);

    return clu_trie_get_handler_rec(t, j, false);
}

bool clu_trie_contains(trie_p t, handler_p h)
{
    assert(t);
    assert(h);

    return clu_trie_contains_rec(t, h, 0);
}
