FLAGS = -c -Wall -Wextra -Werror -Wfatal-errors -O3

LIB_ROOT = $(word 1,$(subst lib, ,$(CURDIR)))lib
DIR = lib$(word 2,$(subst lib, ,$(CURDIR)))

build: lib.o



lib.o: code.c
	echo "building clu object $(DIR)"
	gcc -o $@ $^ $(FLAGS)

debug.o: code.c
	echo "building clu debug $(DIR)"
	gcc -o $@ $^ $(FLAGS) -D DEBUG



clean c:
	$(MAKE) clean --directory=$(LIB_ROOT) -s

_clean:
	echo "cleaning $(DIR)"
	rm -f *.o



.PHONY: test
test t:
_test:

export
