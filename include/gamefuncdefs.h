#ifndef _GAMEFUNCDEFS_H_
#define _GAMEFUNCDEFS_H_

#include <raylib.h>
#include "pac.h"
#include "ghost.h"

int floatCompare(float f1, float f2);
void drawGridLines();
void drawMaze(char maze[MAZE_HEIGHT][MAZE_WIDTH]);
void resetGame(Ghost ghosts[4], Texture2D ghost_sprite_array[6], Player *pacman, Texture2D pacman_narrow);

#endif