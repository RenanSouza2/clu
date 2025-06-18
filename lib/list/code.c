#include <stdlib.h>

#include "debug.h"
#include "../trie/header.h"
#include "../mem/header.h"

#include "../../mods/macros/assert.h"
#include "../../mods/macros/U64.h"


#ifdef DEBUG

#include "../trie/debug.h"
#include "../tag/debug.h"

list_p clu_list_create_variadic_item(va_list *args)
{
    tag_t tag = va_arg(*args, tag_t);
    list_p l = clu_list_create(&tag, NULL);
    int n = va_arg(*args, int);
    l->t = clu_trie_create_variadic_list(n, args);
    assert(l->t);
    return l;
}

list_p clu_list_create_variadic(uint64_t n, va_list *args)
{
    if(n == 0)
        return NULL;

    list_p l, l_first;
    l = l_first = clu_list_create_variadic_item(args);
    for(uint64_t i=1; i<n; i++)
        l = l->next = clu_list_create_variadic_item(args);

    return l_first;
}

list_p clu_list_create_immed(uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    return clu_list_create_variadic(n, &args);
}



bool clu_list_inner(list_p l_1, list_p l_2)
{
    for(uint64_t i=0; l_1 && l_2; i++)
    {
        if(!clu_tag(&l_1->tag, &l_2->tag))
        {
            printf("\n\tLIST ASSERT ERROR\t| TAG MISMATCH | INDEX " U64P() "", i);
            return false;
        }

        if(!clu_trie_inner(l_1->t, l_2->t))
        {
            printf("\n\tLIST ASSERT ERROR\t| TRIE MISMATCH | " U64P() "", i);
            return false;
        }

        l_1 = l_1->next;
        l_2 = l_2->next;
    }

    if(l_2)
    {
        printf("\n\tLIST ASSERT ERROR\t| LIST SHORTER");
        return false;
    }

    if(l_1)
    {
        printf("\n\tLIST ASSERT ERROR\t| LIST LONGER");
        return false;
    }

    return true;
}

bool clu_list(list_p l_1, list_p l_2)
{
    if(!clu_list_inner(l_1, l_2))
    {
        clu_list_report(l_1, "l_1", true);
        clu_list_report(l_2, "l_2", true);
        return false;
    }

    clu_list_free(l_1);
    clu_list_free(l_2);
    return true;
}

bool clu_list_immed(list_p l, uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    list_p l_2 = clu_list_create_variadic(n, &args);
    return clu_list(l, l_2);
}

#endif



void clu_list_report(list_p l, char tag[], bool full)
{
    printf("\n\tCLU REPORT: %s", tag);
    if(l == NULL)
    {
        printf("\n\nEMPTY LIST");
        return;
    }

    for(; l; l = l->next)
    {
        if(full)
        {
            printf("\n%s", l->tag.str);
            clu_trie_display(l->t);
        }
        else
        {
            uint64_t count = clu_trie_count(l->t);
            printf("\n%s: " U64P() "", l->tag.str, count);
        }
    }
}



list_p clu_list_create(tag_p tag, list_p next)
{
    list_p l;
    CALLOC(l, list);

    l->tag = *tag;
    l->next = next;
    return l;
}

list_p clu_list_pop(list_p l)
{
    assert(l);

    list_p l_aux = l->next;
    FREE(l, list);
    return l_aux;
}



void clu_list_free(list_p l_root)
{
    for(list_p l = l_root; l; l = clu_list_pop(l))
        clu_trie_free(l->t);
}

bool clu_list_insert(list_p *l_root, tag_p tag, handler_p h)
{
    assert(l_root);
    assert(tag);
    assert(h);

    if(clu_list_contains(*l_root, h))
        return false;

    for(list_p l = *l_root; l; l = l->next)
    {
        if(!clu_tag_eq(&l->tag, tag))
            continue;

        assert(clu_trie_insert(&l->t, h));
        return true;
    }

    list_p l = clu_list_create(tag, *l_root);
    assert(clu_trie_insert(&l->t, h));
    *l_root = l;
    return true;
}

bool clu_list_remove(list_p *l_root, handler_p h)
{
    assert(l_root);
    assert(h);


    list_p l = *l_root;
    if(l == NULL)
        return false;
        
    if(!clu_trie_remove(&l->t, h))
        return clu_list_remove(&l->next, h);

    if(l->t == NULL)
        *l_root = clu_list_pop(l);

    return true;
}



uint64_t clu_list_count(list_p l)
{
    uint64_t i = 0;
    for(; l; i++)
        l = l->next;

    return i;
}

trie_p clu_list_get_trie(list_p l, uint64_t i)
{
    if(l == NULL)
        return NULL;

    if(i == 0)
        return l->t;

    return clu_list_get_trie(l->next, i-1);
}

bool clu_list_contains(list_p l, handler_p h)
{
    for(; l; l = l->next)
        if(clu_trie_contains(l->t, h))
            return true;

    return false;
}
