.PHONY: test clean

test: runner
	echo "running test $(DIR)"
	./runner

# clu has no aggregate debug object; its lib/debug.o is the whole library.
runner: test.o $(DBG_FILE)
	echo "building test $(DIR)"
	gcc -o $@ $^ $(FLAGS) $(FLAGS_DBG) $(FLAGS_EXE) $(FLAGS_EXTRA)

test.o: test.c
	echo "building test object $(DIR)"
	gcc -o $@ $< $(FLAGS) $(FLAGS_DBG) $(FLAGS_CMP) $(FLAGS_EXTRA)

-include test.d

clean:
	rm -rf test.o test.d runner runner.dSYM
