FLAGS = -std=c11 -Wall -Wextra -Werror -Wfatal-errors -Wpedantic -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wformat=2 -Wnull-dereference -Wconversion -Wsign-conversion -D_POSIX_C_SOURCE=200809L -O2 -march=native -fstack-protector-strong -fstack-clash-protection

LIB = ../lib/lib.o
SRC = ../src/code.o



build: clu.o

clu.o: $(LIB) $(SRC)
	echo " linking clu"
	gcc -r -o $@ $^



.PHONY: $(SRC)
$(SRC):
	$(MAKE) --directory=$(dir $@)

.PHONY: $(LIB)
$(LIB):
	$(MAKE) --directory=$(dir $@)

clean _clean:
	echo "cleaning clu"
	rm -rf *.o
	$(MAKE) clean --directory=$(dir $(LIB))
	$(MAKE) clean --directory=$(dir $(SRC))
