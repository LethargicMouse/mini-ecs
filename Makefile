run: out
	./out
HEADERS := $(wildcard src/*.h)
out: src/main.cpp $(HEADERS)
	g++ -g -Ilib/json/single_include -o out src/*.cpp
