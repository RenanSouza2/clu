FLAGS = -Wall -Wextra -Werror -Wfatal-errors -O3 -c

LIB = ../lib/lib.o
SRC = ../src/code.o

build: bin.o

bin.o: $(LIB) $(SRC)
	echo " linking clu"
	ld -r -o $@ $^

.PHONY: $(SRC) $(LIB)
$(SRC) $(LIB):
	$(MAKE) --directory=$(dir $@)

clean:
	echo "cleaning clu"
	rm -rf *.o
	$(MAKE) clean --directory=..
