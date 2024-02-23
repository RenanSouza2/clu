FLAGS = -c -Wall -Werror -Wfatal-errors

BIN = ../bin/bin.o

DIR = $(shell basename $(CURDIR))

build: code.o

code.o: code.c
	echo "building clu object $(DIR)"
	gcc -o $@ $^ $(FLAGS)

clean:
	rm -rf *.o
