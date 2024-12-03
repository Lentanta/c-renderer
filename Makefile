build:
	echo "Create dist folder and compile all C code"
	mkdir dist
	gcc -Wall -std=c99 ./src/*.c -lSDL2 -lm -o ./dist/renderer

	# NOTE:
	# -lm for linking the math lib
	# -lSDL2 for linking the SDL2 lib

run:
	./dist/renderer

clean:
	rm -rf dist

all: build run clean
