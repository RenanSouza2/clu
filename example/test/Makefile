FLAGS = -Wall -Wextra -Werror -Wfatal-errors -O3
DEBUG_FILE = ../$(LIB_ROOT)/debug.o

test: test.o
	./test.o

test.o: test.c $(DEBUG_FILE)
	echo "building test $(DIR)"
	gcc -o $@ $^ $(FLAGS) -D DEBUG

.PHONY: $(DEBUG_FILE)
$(DEBUG_FILE):
	$(MAKE) debug.o --directory=$(dir $@)

clean:
	rm -rf test.o
