include $(shell git rev-parse --show-toplevel)/makefiles/flags.mk



DIRS := $(wildcard */.)

LIB_FILES = $(foreach DIR,$(DIRS),$(dir $(DIR))lib.o)
DGB_FILES = $(foreach DIR,$(DIRS),$(dir $(DIR))debug.o)

DIR = $(shell basename $(CURDIR))



build: lib.o

dbg: debug.o



lib.o: $(LIB_FILES)
	echo " linking clu object $(DIR)"
	gcc -r $(FLAGS) $(FLAGS_PRD) -o $@ $^ -flto -nostdlib

debug.o: $(DGB_FILES)
	echo " linking clu debug $(DIR)"
	gcc -r $(FLAGS) $(FLAGS_DBG)  -o $@ $^ -flto -nostdlib



.PHONY: $(LIB_FILES)
$(LIB_FILES):
	$(MAKE) --directory=$(dir $@)

.PHONY: $(DGB_FILES)
$(DGB_FILES):
	$(MAKE) dbg --directory=$(dir $@)



clean c:
	$(MAKE) _clean -s

_clean:
	echo "cleaning clu $(DIR)"
	rm -rf *.o
	for DIR in $(DIRS) ; do \
		( $(MAKE) _clean --directory=$$DIR ) || exit $$? ; \
	done



.PHONY: test
test t:
	$(MAKE) dbg -s
	$(MAKE) _test -s

_test:
	for DIR in $(DIRS) ; do \
		( $(MAKE) _test --directory=$$DIR ) || exit $$? ; \
	done
