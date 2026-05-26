ROOT = $(shell git rev-parse --show-toplevel)
include $(ROOT)/makefiles/flags.mk
include $(ROOT)/makefiles/vars.mk



build b: code.o

code.o: code.c
	echo "building $(PRJ_NAME) $(DIR)"
	gcc -o $@ $^ $(FLAGS) $(FLAGS_PRD) $(FLAGS_CMP) -D DEBUG

clean c:
	echo "cleaning $(PRJ_NAME) $(DIR)"
	rm -rf *.o
