SDL2=$(shell sdl2-config --cflags --libs)

all:
	gcc -o main ./src/chip8.c ./src/main.c ./src/graphics.c ./src/events.c $(SDL2)
