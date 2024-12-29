#include <raylib.h>
#include "gamedefs.h"
#include "gamefuncdefs.h"
#include "pac.h"

void initPacman(Player *pacman, Texture2D pacman_narrow, int score, int life_count)
{
    pacman->pacman_sprite_rec = (Rectangle){0, 0, 30, 30};
    pacman->pacman_rec = (Rectangle){(TILE_SIZE * 14) - TILE_SIZE / 2, (TILE_SIZE * 18) - TILE_SIZE / 2, TILE_SIZE, TILE_SIZE};
    pacman->pacman_rotation = 0.0f;
    pacman->pacman_direction = stop;
    pacman->buffer_direction = stop;
    pacman->prev_direction = move_right;
    pacman->pacman_sprite = pacman_narrow;
    pacman->pacman_origin = (Vector2){pacman->pacman_rec.width / 2, pacman->pacman_rec.height / 2};
    pacman->frame_counter = 0;
    pacman->score = score;
    pacman->life_count = life_count;
}

void getPacmanSprite(Player *pacman, Texture2D pacman_wide, Texture2D pacman_narrow, int frame_toggle)
{
    if (pacman->pacman_direction == stop)
        return;

    if (pacman->frame_counter == FRAMERATE / (frame_toggle * 2))
        pacman->pacman_sprite = pacman_wide;

    if (pacman->frame_counter == FRAMERATE / frame_toggle)
    {

        pacman->pacman_sprite = pacman_narrow;
        pacman->frame_counter = 0;
    }

    pacman->frame_counter = pacman->frame_counter + 1;
}

void movePlayer(Player *pacman, char maze[MAZE_HEIGHT][MAZE_WIDTH], Vector2 speed)
{
    Vector2 error_term = {0, 0};

    switch (pacman->pacman_direction)
    {

    case move_up:
    {
        error_term.x = 0.0f;
        error_term.y = -16.0f;
        break;
    }
    case move_left:
    {
        error_term.x = -16.0f;
        error_term.y = 0.0f;
        break;
    }
    case move_down:
    {
        error_term.x = 0.0f;
        error_term.y = 16.0f;
        break;
    }
    case move_right:
    {
        error_term.x = 16.0f;
        error_term.y = 0.0f;
        break;
    }
    case stop:
        break;
    }

    if (maze[(int)((pacman->pacman_rec.y + error_term.y) / TILE_SIZE)][(int)((pacman->pacman_rec.x + error_term.x) / TILE_SIZE)] == '#')
    {

        pacman->pacman_direction = stop;
        return;
    }

    pacman->pacman_rec.x += speed.x;

    pacman->pacman_rec.y += speed.y;

    if (pacman->pacman_rec.x >= 28 * TILE_SIZE - TILE_SIZE / 2 && (pacman->pacman_rec.y <= 15 * TILE_SIZE && pacman->pacman_rec.y >= 14 * TILE_SIZE))
    {
        pacman->pacman_rec.x = TILE_SIZE - TILE_SIZE / 2;
        pacman->pacman_rec.y = 15 * TILE_SIZE - TILE_SIZE / 2;
        return;
    }

    if (pacman->pacman_rec.x <= 0 && (pacman->pacman_rec.y <= 15 * TILE_SIZE && pacman->pacman_rec.y >= 14 * TILE_SIZE))
    {
        pacman->pacman_rec.x = 28 * TILE_SIZE - TILE_SIZE / 2;
        pacman->pacman_rec.y = 15 * TILE_SIZE - TILE_SIZE / 2;
        return;
    }
}

void rotatePlayer(Player *pacman)
{

    switch (pacman->pacman_direction)
    {

    case move_up:
        pacman->pacman_rotation = -90.0f;
        break;
    case move_left:
        pacman->pacman_rotation = 180.0f;
        break;
    case move_down:
        pacman->pacman_rotation = 90.0f;
        break;
    case move_right:
        pacman->pacman_rotation = 0.0f;
        break;
    case stop:
        break;
    }
}

void pacMove(Player *pacman, char maze[MAZE_HEIGHT][MAZE_WIDTH])
{
    Vector2 speed = {PACMAN_SPEED, PACMAN_SPEED};
    float error_term = 14;
    Vector2 error_term_vec;
    Vector2 secondary_error_vec; // Sphagetti to avoid redundancy and improve readability

    if (IsKeyPressed(KEY_W) || pacman->buffer_direction == move_up)
    {
        pacman->buffer_direction = move_up;

        error_term_vec.y = -TILE_SIZE;
        error_term_vec.x = -error_term;

        secondary_error_vec.y = -TILE_SIZE;
        secondary_error_vec.x = error_term;
    }

    if (IsKeyPressed(KEY_A) || pacman->buffer_direction == move_left)
    {
        pacman->buffer_direction = move_left;

        error_term_vec.y = -error_term;
        error_term_vec.x = -TILE_SIZE;

        secondary_error_vec.y = error_term;
        secondary_error_vec.x = -TILE_SIZE;
    }

    if (IsKeyPressed(KEY_S) || pacman->buffer_direction == move_down)
    {
        pacman->buffer_direction = move_down;

        error_term_vec.y = TILE_SIZE;
        error_term_vec.x = -error_term;

        secondary_error_vec.y = TILE_SIZE;
        secondary_error_vec.x = error_term;
    }

    if (IsKeyPressed(KEY_D) || pacman->buffer_direction == move_right)
    {
        pacman->buffer_direction = move_right;

        error_term_vec.y = -error_term;
        error_term_vec.x = TILE_SIZE;

        secondary_error_vec.y = error_term;
        secondary_error_vec.x = TILE_SIZE;
    }

    if (maze[(int)((pacman->pacman_rec.y + error_term_vec.y) / TILE_SIZE)][(int)((pacman->pacman_rec.x + error_term_vec.x) / TILE_SIZE)] != '#')
    {
        if (maze[(int)((pacman->pacman_rec.y + secondary_error_vec.y) / TILE_SIZE)][(int)((pacman->pacman_rec.x + secondary_error_vec.x) / TILE_SIZE)] != '#')
        {
            pacman->pacman_direction = pacman->buffer_direction;
            pacman->prev_direction = pacman->pacman_direction;
        }
    }

    switch (pacman->pacman_direction)
    {

    case move_up:
    {
        speed.x = 0;
        speed.y = -speed.y;
        break;
    }

    case move_left:
    {
        speed.x = -speed.x;
        speed.y = 0;
        break;
    }

    case move_down:
    {
        speed.x = 0;
        break;
    }

    case move_right:
    {

        speed.y = 0;
        break;
    }

    case stop:
    {
        speed.x = 0;
        speed.y = 0;
    }
    }

    movePlayer(pacman, maze, speed);
    rotatePlayer(pacman);
}