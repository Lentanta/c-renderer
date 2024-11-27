build:
	echo "Create dist folder and compile all C code"
	mkdir dist
	gcc -Wall -std=c99 ./src/*.c -lSDL2 -o ./dist/renderer

run:
	./dist/renderer

clean:
	rm -rf dist

all: build run clean
