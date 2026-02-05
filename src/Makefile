include $(shell git rev-parse --show-toplevel)/makefiles/flags.mk

DIR = $(shell basename $(CURDIR))



build b: code.o

code.o: code.c
	echo "building clu $(DIR)"
	gcc -o $@ $^ $(FLAGS) $(FLAGS_PRD) -c -D DEBUG

clean c:
	echo "cleaning clu $(DIR)"
	rm -rf *.o
