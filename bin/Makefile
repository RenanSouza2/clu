PRJ_DIR := $(shell git rev-parse --show-toplevel)
include $(PRJ_DIR)/makefiles/flags.mk
include $(PRJ_DIR)/makefiles/vars.mk



build b: clu.o



clu.o: $(SRC_FILE) $(LIB_FILE)
	echo " linking $(PRJ_NAME) $(DIR)"
	gcc -o $@ $^ $(FLAGS) $(FLAGS_PRD) $(FLAGS_LNK) $(FLAGS_EXTRA)



# FORCE, not .PHONY: these stay real files so the link above fires only when a
# child object actually changed.
FORCE:

$(SRC_FILE) $(LIB_FILE): FORCE
	$(MAKE) --directory=$(dir $@)



clean c _clean:
	echo "cleaning $(PRJ_NAME) $(DIR)"
	rm -f *.o *.d
	$(MAKE) clean --directory=$(LIB_DIR)
	$(MAKE) clean --directory=$(SRC_DIR)
