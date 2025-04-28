#include "./lib/mem/header.h"
#include "./mods/macros/assert.h"

#define TEST_ASSERT_MEM_EMPTY assert(clu_mem_internal_empty());
#define TEST_CASE_TIMEOUT 5
