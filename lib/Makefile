DIRS := $(wildcard */.)

LIB_FILES = $(foreach DIR,$(DIRS),$(dir $(DIR))lib.o)
DGB_FILES = $(foreach DIR,$(DIRS),$(dir $(DIR))debug.o)

DIR = $(shell basename $(CURDIR))

build: lib.o



lib.o: $(LIB_FILES)
	echo "linking clu $(DIR) object"
	ld -r -o $@ $^
	
debug.o: $(DGB_FILES)
	echo "linking $(DIR) debug"
	ld -r -o $@ $^



.PHONY: $(LIB_FILES)
$(LIB_FILES):
	$(MAKE) --directory=$(dir $@)

.PHONY: $(DGB_FILES)
$(DGB_FILES):
	$(MAKE) debug.o --directory=$(dir $@)



.PHONY: clean
clean c:
	echo "cleaning $(DIR)"
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
