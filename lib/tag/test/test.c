#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "../debug.h"
#include "../../mem/debug.h"

void test_tag_convert()
{
    printf("\n\t\t%s\t\t", __func__);

    printf("\n\t\t\t%s 1\t\t", __func__);
    tag_t tag = mem_tag_convert("test");
    assert(strcmp(tag.str, "test") == 0);
    
    printf("\n\t\t\t%s 2\t\t", __func__);
    tag = mem_tag_convert("0123456789012345678901234567890123456789012345678");
    assert(strcmp(tag.str, "0123456789012345678901234567890123456789012345678") == 0);

    assert(mem_mem_empty());
}

void test_tag_eq()
{
    printf("\n\t\t%s\t\t", __func__);

    tag_t tag1, tag2;
    tag1 = mem_tag_convert("test");
    tag2 = mem_tag_convert("teste");
    assert(mem_tag_eq(&tag1, &tag2) == false);
    
    tag2 = mem_tag_convert("test");
    assert(mem_tag_eq(&tag1, &tag2) == true);
    
    tag1 = mem_tag_convert("0123456789012345678901234567890123456789012345678");
    tag2 = mem_tag_convert("0123456789012345678901234567890123456789012345678");
    assert(mem_tag_eq(&tag1, &tag2) == true);

    assert(mem_mem_empty());
}

void test_tag()
{
    printf("\n\t%s\t\t", __func__);

    test_tag_convert();
    test_tag_eq();

    assert(mem_mem_empty());
}

int main() 
{
    setbuf(stdout, NULL);
    test_tag();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
