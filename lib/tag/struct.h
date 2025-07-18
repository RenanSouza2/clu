#ifndef __TAG_STRUCT_H__
#define __TAG_STRUCT_H__

#include "../../mods/macros/struct.h"

#include <stdarg.h>

#define CLU_TAG_SIZE 100

STRUCT(tag)
{
    char str[CLU_TAG_SIZE];
};

tag_t clu_tag_format(char const format[], ...);
tag_t clu_tag_format_variadic(char const format[], va_list args);

#endif
