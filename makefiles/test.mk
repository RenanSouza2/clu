FLAGSs = $(filter-out -Wstack-usage=1024,$(FLAGS))

test: runner
	echo "running test $(DIR)"
	./runner

runner: test.o $(DBG_FILE)
	echo "building test $(DIR)"
	gcc -o $@ $^ $(FLAGSs) $(FLAGS_DBG) $(FLAGS_EXE)

test.o: test.c
	echo "building test object $(DIR)"
	gcc -o $@ $^ $(FLAGSs) $(FLAGS_DBG) $(FLAGS_CMP)

clean:
	rm -rf test.o runner
