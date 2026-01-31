LIB = lib
BIN = bin



build b:
	$(MAKE) --directory=$(BIN) -s

clean c:
	$(MAKE) clean --directory=$(BIN) -s

test t:
	$(MAKE) test --directory=$(LIB) -s
