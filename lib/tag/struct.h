#ifndef __TAG_STRUCT_H__
#define __TAG_STRUCT_H__

#include "header.h"

#include <stdarg.h>

#define TAG_SIZE 50

STRUCT(tag)
{
    char str[TAG_SIZE];
};

tag_t clu_tag_format(char const format[], ...);
tag_t clu_tag_format_variadic(char const format[], va_list args);

#endif
