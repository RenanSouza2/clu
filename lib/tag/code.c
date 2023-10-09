#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>

#include "debug.h"

#ifdef DEBUG
#endif



int mem_tag_len(char const tag_s[])
{
    int len = strlen(tag_s);
    assert(len < TAG_SIZE);
    return len;
}

tag_t mem_tag_convert_variadic(char const format[], va_list args)
{
    tag_t tag;
    memset(&tag, 0, TAG_SIZE);
    vsnprintf(tag.str, TAG_SIZE, format, args);
    return tag;
}

tag_t mem_tag_convert(char const format[], ...)
{
    va_list args;
    va_start(args, format);

    return mem_tag_convert_variadic(format, args);
}

bool mem_tag_eq(tag_p tag1, tag_p tag2)
{
    return memcmp(tag1, tag2, TAG_SIZE) == 0;
}
