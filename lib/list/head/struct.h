#ifndef __LIST_HEAD_STRUCT_H__
#define __LIST_HEAD_STRUCT_H__

#include "header.h"
#include "../body/header.h"
#include "../../tag/struct.h"

STRUCT(list_head)
{
    list_head_p lh;
    list_body_p lb;
    tag_t tag;
};

#endif
