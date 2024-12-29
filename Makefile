CC = gcc -g -Iinclude

all: PACMAN GHOST PELLETS MAIN

PACMAN: src/pac.c
	@${CC} -c src/pac.c -o build/pac.o

GHOST: src/ghost.c
	@${CC} -c src/ghost.c -o build/ghost.o

PELLETS: src/pellets.c
	@${CC} -c src/pellets.c -o build/pellets.o

MAIN: build/pac.o build/ghost.o build/pellets.o
	@${CC} ${CFLAGS} src/main.c build/pac.o build/ghost.o build/pellets.o -o Pacman -lraylib