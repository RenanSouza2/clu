LIB = lib
BIN = bin
SRC = src

DIR = lib$(word 2,$(subst lib, ,$(CURDIR)))

build b:
	$(MAKE) --directory=$(BIN) -s

clean c:
	rm -rf $(TGT)
	$(MAKE) clean --directory=$(LIB) -s
	$(MAKE) clean --directory=$(SRC) -s

test t:
	$(MAKE) test --directory=$(LIB) -s
