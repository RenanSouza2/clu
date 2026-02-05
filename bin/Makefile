ROOT = $(shell git rev-parse --show-toplevel)
include $(ROOT)/makefiles/flags.mk
include $(ROOT)/makefiles/vars.mk



SRC = ../src/code.o



build: clu.o

clu.o: $(SRC) $(LIB_FILE)
	echo " linking clu"
	gcc -r $(FLAGS) $(FLAGS_PRD) -o $@ $^ -flto -nostdlib



.PHONY: $(SRC) $(LIB_FILE)
$(SRC) $(LIB_FILE):
	$(MAKE) --directory=$(dir $@)

clean _clean:
	echo "cleaning clu"
	rm -rf *.o
	$(MAKE) clean --directory=$(dir $(LIB_FILE))
	$(MAKE) clean --directory=$(dir $(SRC))
