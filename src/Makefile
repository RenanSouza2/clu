PRJ_DIR := $(shell git rev-parse --show-toplevel)
include $(PRJ_DIR)/makefiles/flags.mk
include $(PRJ_DIR)/makefiles/vars.mk



build b: code.o



code.o: code.c
	echo "building $(PRJ_NAME) $(DIR)"
	gcc -o $@ $< $(FLAGS) $(FLAGS_PRD) $(FLAGS_CMP) $(FLAGS_EXTRA) -D DEBUG

-include code.d



clean c:
	echo "cleaning $(PRJ_NAME) $(DIR)"
	rm -f *.o *.d
