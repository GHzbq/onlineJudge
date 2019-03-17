all: test


main: main.cpp httplib.h compiler.hpp
	g++ main.cpp -o $@ -g -std=c++11 -lpthread -ljsoncpp
test: test.cpp httplib.h
	g++ test.cpp -o $@ -std=c++11 -lpthread -ljsoncpp

.PHONY: clean

clean: test
	rm -f $^
