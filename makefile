#.PHONY: all
all: deps main

all-clean: clean deps main
	
main:
	g++ -c src/main.cpp -I./lib/cmdBuffer -o build/main.o
	g++ -o main.exe build/main.o -I./lib/cmdBuffer -L./build/lib -lcmdBuffer 

deps: 
	g++ -o build/lib/cmdBuffer.o -c -I./lib/cmdBuffer lib/cmdBuffer/cmdBuffer.cpp
	ar rcs build/lib/libcmdBuffer.a build/lib/cmdBuffer.o

clean:
	rm -rf main.exe
	rm -rf ./build/*.o
	rm -rf ./build/lib/*.o
	rm -rf ./build/lib/*.a