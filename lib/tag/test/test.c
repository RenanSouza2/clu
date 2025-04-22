#include <string.h>

#include "../debug.h"
#include "../../mem/header.h"

#include "../../../mods/macros/test.h"


#define TEST_ASSERT_EMPTY assert(clu_mem_internal_empty());

void test_tag_format(bool show)
{
    TEST_FN

    TEST_CASE_OPEN(1)
    {
        tag_t tag = clu_tag_format("test");
        assert(strncmp(tag.str, "test", CLU_TAG_SIZE) == 0);
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(2)
    {
        tag_t tag = clu_tag_format("012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678");
        assert(strncmp(tag.str, "012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678", CLU_TAG_SIZE) == 0);
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(3)
    {
        tag_t tag = clu_tag_format("0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789");
        assert(strncmp(tag.str, "012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678", CLU_TAG_SIZE) == 0);
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(4)
    {
        tag_t tag = clu_tag_format("%d", 1);
        assert(strncmp(tag.str, "1", CLU_TAG_SIZE) == 0);
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(5)
    {
        tag_t tag = clu_tag_format("%s", "aaa");
        assert(strncmp(tag.str, "aaa", CLU_TAG_SIZE) == 0);
    }
    TEST_CASE_CLOSE

    TEST_ASSERT_EMPTY
}

void test_tag_eq(bool show)
{
    TEST_FN

    TEST_CASE_OPEN(1)
    {
        tag_t tag_1 = clu_tag_format("test");
        tag_t tag_2 = clu_tag_format("teste");
        assert(clu_tag_eq(&tag_1, &tag_2) == false);
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(2)
    {
        tag_t tag_1 = clu_tag_format("test");
        tag_t tag_2 = clu_tag_format("test");
        assert(clu_tag_eq(&tag_1, &tag_2) == true);
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(3)
    {
        tag_t tag_1 = clu_tag_format("0123456789012345678901234567890123456789012345678");
        tag_t tag_2 = clu_tag_format("0123456789012345678901234567890123456789012345678");
        assert(clu_tag_eq(&tag_1, &tag_2) == true);
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(4)
    {
        tag_t tag_1 = clu_tag_format("");
        tag_t tag_2 = clu_tag_format("1");
        assert(clu_tag_eq(&tag_1, &tag_2) == false);
    }
    TEST_CASE_CLOSE

    TEST_ASSERT_EMPTY
}



void test_tag()
{
    TEST_LIB

    bool show = false;

    test_tag_format(show);
    test_tag_eq(show);

    TEST_ASSERT_EMPTY
}



int main()
{
    setbuf(stdout, NULL);
    TEST_TIMEOUT_OPEN(TEST_TIMEOUT_DEFAULT)
    test_tag();
    TEST_TIMEOUT_CLOSE
    printf("\n\n\tTest successful\n\n");
    return 0;
}
