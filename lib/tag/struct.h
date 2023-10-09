#ifndef __TAG_STRUCT_H__
#define __TAG_STRUCT_H__

#include "header.h"

#define TAG_SIZE 50

STRUCT(tag)
{
    char str[TAG_SIZE];
};

tag_t mem_tag_convert(char const format[], ...);
tag_t mem_tag_convert_variadic(char const format[], va_list args);

#endif
