#ifndef TAG_DEBUG_H
#define TAG_DEBUG_H

#include "header.h"

#ifdef DEBUG

#define TAG(STR) ((tag_p)(STR))

bool clu_tag(tag_p tag_1, tag_p tag_2);

#endif

#endif
