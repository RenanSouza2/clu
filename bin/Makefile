FLAGS = -c -Wall -Werror -Wfatal-errors

OBJ = ../lib/lib.o
SRC = ../src/code.o

DIR = $(shell basename $(CURDIR))

build: bin.o

bin.o: $(SRC) $(OBJ)
	echo "linking clu $(DIR) object"
	ld -r -o $@ $^

.PHONY: $(SRC) $(OBJ)
$(SRC) $(OBJ):
	$(MAKE) --directory=$(dir $@)

clean:
	echo "cleaning clu bin"
	rm -rf *.o
	$(MAKE) clean --directory=..
