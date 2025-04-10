
## PACMAN-PROJECT

Implementation of the pacman game(by Toru Iwatani and co) in C with raylib used for rendering. This isn't meant to be a faithful recreation of the original, rather it is just functional, to understand the workflow in creating a non-trivial game.

### Description of source files:

`gamefuncs.h` contains all the declarations of the functions in each file 

`general.c` contains functions common to all entities/the game itself

`ghosts.c` contain all ghost functions

`pac.c` contains all functions pertaining to the player

`pellets.c` contains all pellet functions

`main.c` contains the main game loop

**NOTE**: The resources folder contains all the sprites, and highscores.txt stores the highscores.

### Tools and Libraries used

* gcc
* raylib
* make

### Instructions for compilation:

Refer to the Makefile

### Credits

* https://github.com/raysan5/raylib

