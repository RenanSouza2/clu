#include <string.h>

#include "../debug.h"
#include "../../mem/header.h"
#include "../../../utils/assert.h"



void test_tag_format(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    tag_t tag = clu_tag_format("test");
    assert(strncmp(tag.str, "test", TAG_SIZE) == 0);

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    tag = clu_tag_format("0123456789012345678901234567890123456789012345678");
    assert(strncmp(tag.str, "0123456789012345678901234567890123456789012345678", TAG_SIZE) == 0);

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    tag = clu_tag_format("01234567890123456789012345678901234567890123456789");
    assert(strncmp(tag.str, "0123456789012345678901234567890123456789012345678", TAG_SIZE) == 0);

    if(show) printf("\n\t\t%s 4\t\t", __func__);
    tag = clu_tag_format("%d", 1);
    assert(strncmp(tag.str, "1", TAG_SIZE) == 0);

    if(show) printf("\n\t\t%s 5\t\t", __func__);
    tag = clu_tag_format("%s", "aaa");
    assert(strncmp(tag.str, "aaa", TAG_SIZE) == 0);

    assert(clu_mem_internal_empty());
}

void test_tag_eq(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    tag_t tag_1, tag_2;
    tag_1 = clu_tag_format("test");
    tag_2 = clu_tag_format("teste");
    assert(clu_tag_eq(&tag_1, &tag_2) == false);

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    tag_1 = clu_tag_format("test");
    tag_2 = clu_tag_format("test");
    assert(clu_tag_eq(&tag_1, &tag_2) == true);

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    tag_1 = clu_tag_format("0123456789012345678901234567890123456789012345678");
    tag_2 = clu_tag_format("0123456789012345678901234567890123456789012345678");
    assert(clu_tag_eq(&tag_1, &tag_2) == true);

    if(show) printf("\n\t\t%s 4\t\t", __func__);
    tag_1 = clu_tag_format("");
    tag_2 = clu_tag_format("1");
    assert(clu_tag_eq(&tag_1, &tag_2) == false);

    assert(clu_mem_internal_empty());
}



void test_tag()
{
    printf("\n%s", __func__);

    bool show = false;

    test_tag_format(show);
    test_tag_eq(show);

    assert(clu_mem_internal_empty());
}



int main()
{
    setbuf(stdout, NULL);
    test_tag();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
