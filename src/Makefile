FLAGS = -std=c11 -Wall -Wextra -Werror -Wfatal-errors -Wpedantic -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wformat=2 -Wnull-dereference -Wconversion -Wsign-conversion -D_POSIX_C_SOURCE=200809L -O2 -march=native -fstack-protector-strong -fstack-clash-protection

DIR = lib$(word 2,$(subst lib, ,$(CURDIR)))

build: code.o

code.o: code.c
	echo "building clu object $(DIR)"
	gcc -o $@ $^ $(FLAGS) -c -D DEBUG

clean:
	rm -rf *.o
