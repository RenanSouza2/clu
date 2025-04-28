LIB = lib
BIN = bin

DIR = lib$(word 2,$(subst lib, ,$(CURDIR)))

build b:
	$(MAKE) --directory=$(BIN) -s

clean c:
	$(MAKE) clean --directory=$(BIN) -s

test t:
	$(MAKE) test --directory=$(LIB) -s
