LIB = lib
BIN = bin

build:
	$(MAKE) --directory=$(BIN)

clean:
	rm -rf $(TGT)
	$(MAKE) clean --directory=$(BIN)
	$(MAKE) clean --directory=$(LIB)
	
test: 
	$(MAKE) test --directory=$(LIB) -s
