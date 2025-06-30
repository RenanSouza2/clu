
This tool finds finds memory leaks and double frees



an example on how to install and use is found at: https://github.com/RenanSouza2/c-template

# instructions
Build it with `make` in either the root or bin folders

Inlcude the `bin/header.h` in your project to replace `malloc`, `calloc`, `realloc` and `free` functions so the allocated memory can be tracked

The project in this current state is not advised to be used in production due to performance concerns.

# functions

There are 3 functions that can be used to probe the memory status:
- `clu_mem_report`: displays a report of current allocated pointers
- `clu_mem_report_full`: same but with more details
- `clu_mem_is_empty`: Returns true if there are no allocated pointers


These macros allow you to intereact with the register
- `CLU_HANDLER_IS_SAFE(HANDLER)`: this macro  can be inserted anywhere in the code, it verifies if a given handler is either NULL pointer or an allocated pointer, it does not do anything in production mode and doesn't add any overhead
- `CLU_HANDLER_REGISTER(HANDLER)`: regirsters a given HANDLER
- `CLU_HANDLER_USREGISTER(HANDLER)`: removes a given HANDLER from register


You can get leaked pointer to inspect their content with these functions

- `clu_get_handler(i, j)`: gets a leaked handler, i and j are coordenates related to the full report, the i specifies the index of the tag and the j the index of the handles inside a tag, if i an j are invalis it reverts

- `clu_get_max_i`: gets the biggest valid value of i

- `clu_get_max_j(i)`: gets the biggest valid value of j given an i


Logs
- `clu_set_log(bool)`: set it to true so every handler registered or unregistered is logged