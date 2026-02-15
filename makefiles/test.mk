FLAGSs = $(filter-out -Wstack-usage=1024,$(FLAGS))

test: runner
	echo "running test $(DIR)"
	./runner

runner: test.c $(DBG_FILE)
	echo "building test $(DIR)"
	gcc -o $@ $^ $(FLAGSs) $(FLAGS_DBG) $(FLAGS_EXE)

clean:
	rm -rf runner
