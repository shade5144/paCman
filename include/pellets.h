#ifndef _PELLETS_H_
#define _PELLETS_H_

#include "gamedefs.h"
#include "pac.h"
#include "ghost.h"

void pelletCollision(Player *pacman, char maze[MAZE_HEIGHT][MAZE_WIDTH], int increment);
void powPel(Player *pacman, int *is_frightened, char maze[MAZE_HEIGHT][MAZE_WIDTH], int increment);
int pelletCount(char maze[MAZE_HEIGHT][MAZE_WIDTH]);

#endif