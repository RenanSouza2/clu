LIB = lib
BIN = bin
SRC = src



build b:
	$(MAKE) --directory=$(BIN) -s -j

dbg d:
	$(MAKE) dbg --directory=$(LIB) -s -j

clean c _clean:
	$(MAKE) clean --directory=$(BIN) -s -j

test t:
	$(MAKE) test --directory=$(LIB) -s -j
