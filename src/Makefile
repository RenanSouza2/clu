FLAGS = -std=c11 -Wall -Wextra -Werror -Wfatal-errors -Wpedantic -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wformat=2 -Wnull-dereference -Wconversion -Wsign-conversion -D_POSIX_C_SOURCE=200809L
FLAGS_PRD = -O2 -march=native -fstack-protector-strong

ifeq ($(shell uname -s),Linux)
    FLAGS_PRD += -fstack-clash-protection
endif

DIR = $(shell basename $(CURDIR))



build b: code.o

code.o: code.c
	echo "building clu $(DIR)"
	gcc -o $@ $^ $(FLAGS) $(FLAGS_PRD) -c -D DEBUG

clean c:
	echo "cleaning clu $(DIR)"
	rm -rf *.o
