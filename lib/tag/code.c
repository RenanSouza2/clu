#include <stdio.h>
#include <string.h>

#include "debug.h"

#ifdef DEBUG

bool clu_tag(tag_p tag_1, tag_p tag_2)
{
    if(!clu_tag_eq(tag_1, tag_2))
    {
        printf("\n\tTAG ASSERT ERROR\t| TAGS MISMATCH | (%s) (%s)", tag_1->str, tag_2->str);
        return false;
    }

    return true;
}

#endif



tag_t clu_tag_format_variadic(char const format[], va_list args)
{
    tag_t tag;
    memset(&tag, 0, TAG_SIZE);
    vsnprintf(tag.str, TAG_SIZE, format, args);
    return tag;
}

tag_t clu_tag_format(char const format[], ...)
{
    va_list args;
    va_start(args, format);

    return clu_tag_format_variadic(format, args);
}

bool clu_tag_eq(tag_p tag1, tag_p tag2)
{
    return memcmp(tag1, tag2, TAG_SIZE) == 0;
}
