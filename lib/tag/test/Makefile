FLAGS = -Wall -Wextra -Werror -Wfatal-errors -O3 -fsanitize=address
DEBUG_FILE = $(LIB_ROOT)/debug.o

test: test.o
	./test.o

test.o: test.c $(DEBUG_FILE)
	echo "building test $(DIR)"
	gcc -o $@ $^ $(FLAGS) -D DEBUG

clean:
	rm -rf test.o
