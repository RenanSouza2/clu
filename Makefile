LIB = lib
BIN = bin

build b:
	$(MAKE) --directory=$(BIN)

clean c:
	rm -rf $(TGT)
	$(MAKE) clean --directory=$(BIN)
	$(MAKE) clean --directory=$(LIB)
	
test t: 
	$(MAKE) test --directory=$(LIB) -s
