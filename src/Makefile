FLAGS = -c -Wall -Werror -Wfatal-errors

OBJ = ../lib/lib.o
BIN = ../bin/bin.o

build: $(BIN)

$(BIN): code.o $(OBJ)
	echo "Linking final object"
	ld -r -o $@ $^

code.o: code.c
	echo "Building final object"
	gcc -o $@ $^ $(FLAGS)

.PHONY: $(OBJ)
$(OBJ):
	$(MAKE) --directory=$(dir $(OBJ))

clean:
	rm -rf *.o
