LIB = lib
BIN = bin
SRC = src

build b:
	$(MAKE) --directory=$(BIN)

clean c:
	rm -rf $(TGT)
	$(MAKE) clean --directory=$(BIN)
	$(MAKE) clean --directory=$(LIB)
	$(MAKE) clean --directory=$(SRC)
	
test t: 
	$(MAKE) test --directory=$(LIB) -s
