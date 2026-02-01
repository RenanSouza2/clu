# clu: A C Memory Debugging Library

**clu** is a thread-safe C library designed to help developers find memory leaks, double frees, and other common memory-related errors. It is intended for use during development and debugging, not for production.

An example on how to install and use is found at: https://github.com/RenanSouza2/c-template

## Features

- **Error Detection:** Catches double frees, freeing of non-allocated or static pointers, and reports on memory leaks.
- **Thread-Safe:** Designed to be safely used in multi-threaded applications.
- **Detailed Reporting:** Provides functions to report on current memory usage, including the exact source location (file and line number) of each allocation.
- **Debug-Only Integration:** The library is enabled by a `DEBUG` macro, ensuring it has zero performance overhead when compiled for production.

## Build Instructions

The project uses a `make`-based build system.

- `make build`: Compiles the library, creating a relocatable object `bin/clu.o` that can be linked into your project.
- `make test`: Compiles the library with sanitizers and runs the internal test suite.
- `make clean`: Removes all build artifacts.

## Usage

1.  **Include the Header:** In your C files, include the `clu` header. This will override the standard `malloc`, `calloc`, `realloc`, and `free` functions with the tracked versions.

    ```c
    #include "path/to/your/copy/of/header.h"
    ```

2.  **Compile with `DEBUG` flag:** To enable memory tracking, you must compile your project with the `-D DEBUG` flag.

3.  **Link `clu.o`:** Link the compiled `bin/clu.o` object file with your application.

### Key Functions and Macros

- `clu_mem_report(tag)`: Displays a report of currently allocated pointers, grouped by their allocation site (`tag`).
- `clu_mem_report_full(tag)`: A more detailed version of the report.
- `clu_mem_is_empty()`: Returns `true` if no memory is currently allocated (i.e., no leaks). This is useful for automated checks at the end of a program or test.
- `CLU_HANDLER_IS_SAFE(handler)`: A macro that checks if a handler is a valid, allocated pointer. It has no effect in non-debug builds.
- `clu_log_level_set(level)`: Controls the verbosity of logging for allocations and deallocations.

## Limitations and Pitfalls

- **Development Use Only:** The library introduces performance overhead and is not recommended for use in production builds. Its primary purpose is for debugging.
- **Disruptive Error Handling:** `clu` calls `exit()` upon detecting an error (like a double free). It does not provide a way for the application to handle the error gracefully.
- **`realloc` Handling:** The internal handling of `realloc` is not fully robust and may lead to incorrect error reports if the `realloc` call itself fails.
- **Platform Dependency:** The build system and parts of the code are designed for POSIX-like systems (Linux, macOS) and will not work on Windows without modification.