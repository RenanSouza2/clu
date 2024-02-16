FLAGS = -c -Wall -Werror -Wfatal-errors

BIN = ../bin/bin.o

build: code.o

code.o: code.c
	echo "Building final object"
	gcc -o $@ $^ $(FLAGS)

clean:
	rm -rf *.o
