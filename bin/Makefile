FLAGS = -c -Wall -Wextra -Werror -Wfatal-errors -O3 -O3

OBJ = ../lib/lib.o
SRC = ../src/code.o

build: bin.o

bin.o: $(SRC) $(OBJ)
	echo " linking clu"
	ld -r -o $@ $^

.PHONY: $(SRC) $(OBJ)
$(SRC) $(OBJ):
	$(MAKE) --directory=$(dir $@)

clean:
	echo "cleaning clu"
	rm -rf *.o
	$(MAKE) clean --directory=..
