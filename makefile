all: oj_server

oj_server:oj_server.cpp oj_model.hpp oj_view.hpp
	g++ oj_server.cpp -o oj_server -std=c++11 -lpthread\
		-ljsoncpp -lctemplate 

main: main.cpp httplib.h compiler.hpp
	g++ main.cpp -o $@ -g -std=c++11 -lpthread -ljsoncpp
test: test.cpp httplib.h
	g++ test.cpp -o $@ -std=c++11 -lpthread -ljsoncpp

.PHONY: clean

clean: oj_server
	rm -f $^
