DIRS := $(wildcard */.)

LIB_FILES = $(foreach DIR,$(DIRS),$(dir $(DIR))lib.o)
DGB_FILES = $(foreach DIR,$(DIRS),$(dir $(DIR))debug.o)

DIR = lib$(word 2,$(subst lib, ,$(CURDIR)))

build: lib.o



lib.o: $(LIB_FILES)
	echo " linking clu object $(DIR)"
	ld -r -o $@ $^

debug.o: $(DGB_FILES)
	echo " linking clu debug $(DIR)"
	ld -r -o $@ $^



.PHONY: $(LIB_FILES)
$(LIB_FILES):
	$(MAKE) --directory=$(dir $@)

.PHONY: $(DGB_FILES)
$(DGB_FILES):
	$(MAKE) debug.o --directory=$(dir $@)



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
	$(MAKE) debug.o
	$(MAKE) _test -s

_test:
	for DIR in $(DIRS) ; do \
		( $(MAKE) _test --directory=$$DIR ) || exit $$? ; \
	done
