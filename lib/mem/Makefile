FLAGS = -Wall -Wextra -Werror -Wfatal-errors -O3 -c
FLAGS_DBG = -D DEBUG -fsanitize=address

LIB_ROOT = $(word 1,$(subst lib, ,$(CURDIR)))lib
DIR = lib$(word 2,$(subst lib, ,$(CURDIR)))

build: lib.o



lib.o: code.c
	echo "building object $(DIR)"
	gcc -o $@ $^ $(FLAGS)

debug.o: code.c
	echo "building debug $(DIR)"
	gcc -o $@ $^ $(FLAGS) $(FLAGS_DBG)



clean c:
	$(MAKE) clean --directory=$(LIB_ROOT) -s

_clean:
	echo "cleaning $(DIR)"
	rm -f *.o



_test:

export
