FLAGS = -c -Wall -Werror -Wfatal-errors

OBJ = ../lib/lib.o
MAIN = ../src/code.o

build: bin.o

bin.o: $(MAIN) $(OBJ)
	echo "Linking final object"
	ld -r -o $@ $^

.PHONY: $(MAIN) $(OBJ)
$(MAIN) $(OBJ):
	$(MAKE) --directory=$(dir $@)

clean:
	rm -rf *.o
