LIB = lib
BIN = bin
SRC = src



build b:
	$(MAKE) --directory=$(BIN) -s -j

dbg d:
	$(MAKE) dbg --directory=$(LIB) -s -j

clean c:
	$(MAKE) clean --directory=$(BIN) -s -j

test t:
	$(MAKE) test --directory=$(LIB) -s -j

lint l:
	python3 makefiles/lint_tests.py
