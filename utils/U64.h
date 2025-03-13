#ifndef __U64_H__

#ifdef __linux__

#include <stdint.h>

#define U64P(C) "%" #C "lu"
#define U64PX "%016lx"

#elif defined __APPLE__

#define U64P(C) "%" #C "llu"
#define U64PX "%016llx"

#endif

#endif