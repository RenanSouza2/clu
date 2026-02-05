FLAGS = -std=c11 -Wall -Wextra -Werror -Wfatal-errors -Wpedantic -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wformat=2 -Wnull-dereference -Wconversion -Wsign-conversion -D_POSIX_C_SOURCE=200809L
FLAGS_PRD = -O2 -march=native -fstack-protector-strong
FLAGS_DBG = -D DEBUG -O0 -g3 -ggdb -fno-omit-frame-pointer -fsanitize=address,undefined

ifeq ($(shell uname -s),Linux)
    FLAGS_PRD += -fstack-clash-protection
	FLAGS_DBG += -fsanitize=leak
endif
