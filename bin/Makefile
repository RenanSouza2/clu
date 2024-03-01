FLAGS = -c -Wall -Werror -Wfatal-errors

OBJ = ../lib/lib.o
SRC = ../src/code.o

DIR = lib$(word 2,$(subst lib, ,$(CURDIR)))

build: bin.o

bin.o: $(SRC) $(OBJ)
	echo " linking clu object $(DIR)"
	ld -r -o $@ $^

.PHONY: $(SRC) $(OBJ)
$(SRC) $(OBJ):
	$(MAKE) --directory=$(dir $@)

clean:
	echo "cleaning clu $(DIR)"
	rm -rf *.o
	$(MAKE) clean --directory=..
