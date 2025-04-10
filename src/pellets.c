#include "gamedefs.h"
#include "pac.h"
#include "ghost.h"

void pelletCollision(Player *pacman, char maze[MAZE_HEIGHT][MAZE_WIDTH], int increment)
{

    if (maze[(int)(pacman->pacman_rec.y / TILE_SIZE)][(int)(pacman->pacman_rec.x / TILE_SIZE)] == '.')
    {

        pacman->score += increment;
        maze[(int)(pacman->pacman_rec.y / TILE_SIZE)][(int)(pacman->pacman_rec.x / TILE_SIZE)] = ' ';
    }
}

void powPel(Player *pacman, int *is_frightened, char maze[MAZE_HEIGHT][MAZE_WIDTH], int increment)
{

    if (maze[(int)(pacman->pacman_rec.y / TILE_SIZE)][(int)(pacman->pacman_rec.x / TILE_SIZE)] == 'O')
    {

        pacman->score += increment;
        maze[(int)(pacman->pacman_rec.y / TILE_SIZE)][(int)(pacman->pacman_rec.x / TILE_SIZE)] = ' ';
        *is_frightened = 1;
    }
}

int pelletCount(char maze[MAZE_HEIGHT][MAZE_WIDTH])
{

    int pellets = 0;

    for (int i = 0; i < MAZE_HEIGHT; i++)
    {

        for (int j = 0; j < MAZE_WIDTH; j++)
        {

            if (maze[i][j] == '.' || maze[i][j] == 'O')
                pellets++;
        }
    }

    return pellets;
}
