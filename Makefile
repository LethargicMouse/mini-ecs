run: out
	./out
HEADERS := $(wildcard src/*.h)
out: src/main.cpp $(HEADERS)
	g++ -g -o out src/*.cpp
