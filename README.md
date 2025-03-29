
This tool finds finds memory leaks and double frees



an example on how to install and use is found at: https://github.com/RenanSouza2/c-template

# instructions
Build it with `make` in either the root or bin folders

Inlcude the `bin/header.h` in your project to replace `malloc`,  `calloc`, `realloc` and `free` functions so the allocated memory can be tracked

The project in this current state is not advised to be used in production due to performance concerns.

# functions

There are 4 functions that can be used to probe the memory status:
- `clu_mem_report`: displays a report of current allocated pointers
- `clu_mem_report_full`: same but with more details
- `clu_mem_is_empty`: Returns true if there are no allocated pointers
- `clu_mem_get_handler`: gets an allocated pointer, the coordinates `i` and `j` are related to the full report
