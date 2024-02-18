FLAGS = -c -Wall -Werror -Wfatal-errors

OBJ = ../lib/lib.o
MAIN = ../src/code.o

DIR = clu

build: bin.o

bin.o: $(MAIN) $(OBJ)
	echo "linking $(DIR) object"
	ld -r -o $@ $^

.PHONY: $(MAIN) $(OBJ)
$(MAIN) $(OBJ):
	$(MAKE) --directory=$(dir $@)

clean:
	rm -rf *.o
