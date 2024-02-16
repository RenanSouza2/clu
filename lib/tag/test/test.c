#include <string.h>

#include "../debug.h"
#include "../../mem/header.h"
#include "../../../utils/assert.h"



void test_tag_format()
{
    printf("\n\t\t%s", __func__);

    printf("\n\t\t\t%s 1", __func__);
    tag_t tag = clu_tag_format("test");
    assert(strcmp(tag.str, "test\0") == 0);
    
    printf("\n\t\t\t%s 2", __func__);
    tag = clu_tag_format("0123456789012345678901234567890123456789012345678");
    assert(strcmp(tag.str, "0123456789012345678901234567890123456789012345678\0") == 0);
    
    printf("\n\t\t\t%s 3", __func__);
    tag = clu_tag_format("01234567890123456789012345678901234567890123456789");
    assert(strcmp(tag.str, "0123456789012345678901234567890123456789012345678\0") == 0);

    assert(clu_mem_empty_internal());
}

void test_tag_eq()
{
    printf("\n\t\t%s", __func__);

    tag_t tag1, tag2;
    tag1 = clu_tag_format("test");
    tag2 = clu_tag_format("teste");
    assert(clu_tag_eq(&tag1, &tag2) == false);
    
    tag2 = clu_tag_format("test");
    assert(clu_tag_eq(&tag1, &tag2) == true);
    
    tag1 = clu_tag_format("0123456789012345678901234567890123456789012345678");
    tag2 = clu_tag_format("0123456789012345678901234567890123456789012345678");
    assert(clu_tag_eq(&tag1, &tag2) == true);

    assert(clu_mem_empty_internal());
}

void test_tag()
{
    printf("\n\t%s", __func__);

    test_tag_format();
    test_tag_eq();

    assert(clu_mem_empty_internal());
}

int main() 
{
    setbuf(stdout, NULL);
    test_tag();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
