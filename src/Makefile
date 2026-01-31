FLAGS = -std=c11 -Wall -Wextra -Werror -Wfatal-errors -Wpedantic -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wformat=2 -Wnull-dereference -Wconversion -Wsign-conversion -D_POSIX_C_SOURCE=200809L -O2 -march=native -fstack-protector-strong -fstack-clash-protection

DIR = $(shell basename $(CURDIR))



build b: code.o

code.o: code.c
	echo "building clu $(DIR)"
	gcc -o $@ $^ $(FLAGS) -c -D DEBUG

clean c:
	echo "cleaning clu $(DIR)"
	rm -rf *.o
