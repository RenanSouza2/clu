#ifndef __LIST_HEAD_STRUCT_H__
#define __LIST_HEAD_STRUCT_H__

#include "header.h"
#include "../body/header.h"
#include "../../tag/struct.h"

STRUCT(list_head)
{
    tag_t tag;
    list_body_p lb;
    list_head_p lh;
};

#endif
