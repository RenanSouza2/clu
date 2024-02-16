#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

#include "debug.h"

#ifdef DEBUG
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
