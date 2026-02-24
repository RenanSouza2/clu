ROOT = $(shell git rev-parse --show-toplevel)
include $(ROOT)/makefiles/flags.mk
include $(ROOT)/makefiles/vars.mk



build b: code.o

code.o: code.c
	echo "building clu $(DIR)"
	gcc -o $@ $^ $(FLAGS) $(FLAGS_PRD) -c -D DEBUG

clean c:
	echo "cleaning clu $(DIR)"
	rm -rf *.o
