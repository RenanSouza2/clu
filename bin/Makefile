OBJ = ../lib/lib.o

build: bin.o

bin.o: $(OBJ)
	cp $^ $@

.PHONY: $(OBJ)
$(OBJ):
	$(MAKE) --directory=$(dir $(OBJ))

clean:
	rm -rf bin.o
