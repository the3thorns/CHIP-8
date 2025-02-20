all:
	gcc -o main ./src/chip8.c ./src/main.c ./src/graphics.c -lSDL2