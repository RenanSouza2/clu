PRJ_DIR = $(shell git rev-parse --show-toplevel)
include $(PRJ_DIR)/makefiles/flags.mk
include $(PRJ_DIR)/makefiles/vars.mk



SRC = ../src/code.o



build: clu.o

clu.o: $(SRC) $(LIB_FILE)
	echo " linking $(PRJ_NAME)"
	gcc $(FLAGS) $(FLAGS_PRD) $(FLAGS_LNK) -o $@ $^



.PHONY: $(SRC) $(LIB_FILE)
$(SRC) $(LIB_FILE):
	$(MAKE) --directory=$(dir $@)

clean _clean:
	echo "cleaning $(PRJ_NAME)"
	rm -rf *.o
	$(MAKE) clean --directory=$(dir $(LIB_FILE))
	$(MAKE) clean --directory=$(dir $(SRC))
