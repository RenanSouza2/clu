FLAGS = -c -Wall -Wextra -Werror -Wfatal-errors -O3

DIR = lib$(word 2,$(subst lib, ,$(CURDIR)))

build: code.o

code.o: code.c
	echo "building clu object $(DIR)"
	gcc -o $@ $^ $(FLAGS) -D DEBUG

clean:
	rm -rf *.o
