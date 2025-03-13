FLAGS = -c -Wall -Wextra -Werror -Wfatal-errors -O3 -O3

BIN = ../bin/bin.o

DIR = lib$(word 2,$(subst lib, ,$(CURDIR)))

build: code.o

code.o: code.c
	echo "building clu object $(DIR)"
	gcc -o $@ $^ $(FLAGS)

clean:
	rm -rf *.o
