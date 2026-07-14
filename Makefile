run: out
	./out
out: src/main.cpp
	g++ -g -o out src/*.cpp
