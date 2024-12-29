#ifndef _PAC_H_
#define _PAC_H_

#include <raylib.h>

typedef struct
{
    Texture2D pacman_sprite;
    Rectangle pacman_sprite_rec;
    Rectangle pacman_rec;
    Vector2 pacman_origin;
    Movement_Direction pacman_direction;
    Movement_Direction buffer_direction;
    Movement_Direction prev_direction;
    float pacman_rotation;
    int score;
    int life_count;
    int frame_counter;

} Player;

void initPacman(Player *pacman, Texture2D pacman_narrow, int score, int life_count);
void getPacmanSprite(Player *pacman, Texture2D pacman_wide, Texture2D pacman_narrow, int frame_toggle);
void movePlayer(Player *pacman, char maze[MAZE_HEIGHT][MAZE_WIDTH], Vector2 speed);
void rotatePlayer(Player *pacman);
void pacMove(Player *pacman, char maze[MAZE_HEIGHT][MAZE_WIDTH]);

#endif