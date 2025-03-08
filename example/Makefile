FLAGS = -c -Wall -Werror -Wfatal-errors -O3 -O3

LIB_ROOT = ..
DIR = lib$(word 2,$(subst lib, ,$(CURDIR)))

build: lib.o

lib.o: code.c
	echo "building clu object $(DIR)"
	gcc -o $@ $^ $(FLAGS)

debug.o: code.c
	echo "building clu debug $(DIR)"
	gcc -o $@ $^ $(FLAGS) -D DEBUG

clean:
	rm -f *.o
	$(MAKE) clean --directory=test

.PHONY: test
test:
	$(MAKE) --directory=test

export
