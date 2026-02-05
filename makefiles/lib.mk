include $(shell git rev-parse --show-toplevel)/makefiles/flags.mk

LIB_ROOT = $(shell git rev-parse --show-toplevel)/lib
DIR = $(shell basename $(CURDIR))



build: lib.o

dbg: debug.o



lib.o: code.c
	echo "building clu object $(DIR)"
	gcc -o $@ $^ $(FLAGS) $(FLAGS_PRD) -c

debug.o: code.c
	echo "building clu debug $(DIR)"
	gcc -o $@ $^ $(FLAGS) $(FLAGS_DBG) -c



clean c:
	$(MAKE) clean --directory=$(LIB_ROOT) -s

_clean:
	echo "cleaning clu $(DIR)"
	rm -f *.o
	$(MAKE) clean --directory=test



.PHONY: test
test t:
	$(MAKE) dbg --directory=$(LIB_ROOT) -s
	$(MAKE) _test -s

_test:
	$(MAKE) --directory=test

export
