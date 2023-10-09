FLAGS = -c -Wall -Werror -Wfatal-errors

OBJ = ../lib/lib.o

build: bin.o

bin.o: code.o $(OBJ)
	echo "Linking final object"
	ld -r -o $@ $^

bin.o: code.c $(OBJ)
	echo "Building final object"
	gcc -o $@ $^ $(FLAGS)

.PHONY: $(OBJ)
$(OBJ):
	$(MAKE) --directory=$(dir $(OBJ))

clean:
	rm -rf *.o
