LIB = lib
BIN = bin
SRC = src

DIR = $(shell basename $(CURDIR))

build b:
	echo "linking $(DIR) object"
	$(MAKE) --directory=$(BIN) -s

clean c:
	rm -rf $(TGT)
	$(MAKE) clean --directory=$(LIB) -s
	$(MAKE) clean --directory=$(SRC) -s
	
test t: 
	$(MAKE) test --directory=$(LIB) -s
