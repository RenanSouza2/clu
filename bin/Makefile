include $(shell git rev-parse --show-toplevel)/makefiles/flags.mk

LIB = ../lib/lib.o
SRC = ../src/code.o



build: clu.o

clu.o: $(LIB) $(SRC)
	echo " linking clu"
	gcc -r $(FLAGS) $(FLAGS_PRD) -o $@ $^ -flto -nostdlib



.PHONY: $(SRC) $(LIB)
$(SRC) $(LIB):
	$(MAKE) --directory=$(dir $@)

clean _clean:
	echo "cleaning clu"
	rm -rf *.o
	$(MAKE) clean --directory=$(dir $(LIB))
	$(MAKE) clean --directory=$(dir $(SRC))
