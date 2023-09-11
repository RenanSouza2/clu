LIB = lib
BIN = $(LIB)/lib.o
TGT = bin/lib.o

build: $(TGT)

$(TGT): $(BIN)
	cp $(BIN) $(TGT)

.PHONY: $(BIN)
$(BIN):
	$(MAKE) --directory=$(LIB)
	
clean:
	rm -rf $(TGT)
	$(MAKE) clean --directory=$(LIB)
	
test: 
	$(MAKE) test --directory=$(LIB) -s
