#ifndef _GHOST_H_
#define _GHOST_H_

#include <raylib.h>
#include "gamedefs.h"
#include "pac.h"

typedef enum
{

    blinky,
    inky,
    pinky,
    clyde,

} Ghost_Name;

typedef enum
{
    in_ghosthouse,
    release_ghosthouse,
    scatter,
    chase,
    frightened,
    back_to_ghosthouse,
} Ghost_State;

typedef struct
{

    Ghost_Name name;
    Texture2D ghost_sprite;
    Rectangle ghost_sprite_rec;
    Rectangle ghost_rec;
    Vector2 ghost_origin;
    Vector2 temp_target;
    Vector2 main_target;
    Vector2 start_posn;
    int best_tile;
    int prev_tile;
    Movement_Direction ghost_direction;
    Ghost_State ghost_state;
    Ghost_State prev_ghost_state;
    int is_frightened;
    Vector2 ghost_speed;

} Ghost;

extern float distance_array[4];

void initGhost(Ghost *ghost, Texture2D sprite, float x_posn, float y_posn, Ghost_Name name);
int returnOppositeTile(int tile);
void getSprite(Ghost *ghost, Texture2D sprite_array[6]);
void getDistArray(Ghost *ghost, char maze[MAZE_HEIGHT][MAZE_WIDTH]); // float *getDistArray(Ghost *ghost, char maze[MAZE_HEIGHT][MAZE_WIDTH]);
void mazeMove(Ghost *ghost);
void moveGhost(Ghost *ghost, Vector2 speed);
int returnMinTile(float *distance_array);
int returnRandTile(float *distance_array);
void moveToTarget(Ghost *ghost, char maze[MAZE_HEIGHT][MAZE_WIDTH], Vector2 ghost_speed, Ghost_State prev_ghosts_state);
void setTarget(Ghost *ghost, Ghost ghosts[4], Player pacman, Ghost_State overall_ghosts_state);
void ghostCollision(Ghost *ghost, Player *pacman, Game_State *game_state);

#endif