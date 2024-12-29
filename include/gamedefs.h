#ifndef _GAME_FUNCS_H_
#define _GAME_FUNCS_H_

#define MAZE_HEIGHT 31
#define MAZE_WIDTH 28
#define TILE_SIZE 32
#define PACMAN_SPEED 2
#define GHOST_BASE_SPEED 2
#define GHOST_REDUCED_SPEED 1
#define GHOST_BACK_SPEED 4
#define SCATTER_TIME 5 // 10
#define CHASE_TIME 10  // 36
#define FRIGHTENED_TIME 30
#define DISTANCE_INFINITY 9999
#define FRAMERATE 60

#include <raylib.h>

typedef enum
{
    USER,
    READY,
    GAME_RUNNING,
    LIFE_LOST,
    GAME_OVER,
    WIN,
    HIGH_SCORE,
    CLOSE,

} Game_State;

typedef enum
{

    move_up,
    move_left,
    move_down,
    move_right,
    stop,
} Movement_Direction;

#endif