hello: helloWorld.cpp
	g++ $^ -o $@ -std=c++11 -lpthread

.PHONY: clean

clean: hello
	rm -f $^
