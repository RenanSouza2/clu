#include <stdio.h>
#include <string.h>

#include "debug.h"
#ifdef DEBUG

bool clu_tag(tag_p tag_1, tag_p tag_2)
{
    if(!clu_tag_eq(tag_1, tag_2))
    {
        fprintf(stderr, "\n\n\tTAG ASSERT ERROR\t| (%s) (%s)", tag_1->str, tag_2->str);
        return false;
    }

    return true;
}

#endif


__attribute__((format(printf, 1, 0)))
tag_t clu_tag_format_variadic(char const format[], va_list args)
{
    tag_t tag;
    memset(&tag, 0, CLU_TAG_SIZE);
    vsnprintf(tag.str, CLU_TAG_SIZE, format, args);
    return tag;
}

__attribute__((format(printf, 1, 2)))
tag_t clu_tag_format(char const format[], ...)
{
    va_list args;
    va_start(args, format);

    return clu_tag_format_variadic(format, args);
}

bool clu_tag_eq(tag_p tag_1, tag_p tag_2)
{
    return strncmp(tag_1->str, tag_2->str, CLU_TAG_SIZE) == 0;
}
