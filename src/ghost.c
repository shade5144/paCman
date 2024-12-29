// #include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <raymath.h>
#include "gamedefs.h"
#include "gamefuncdefs.h"
#include "pac.h"
#include "ghost.h"

float distance_array[4];

void initGhost(Ghost *ghost, Texture2D sprite, float x_posn, float y_posn, Ghost_Name name)
{
    ghost->name = name;
    ghost->ghost_sprite = sprite;
    ghost->ghost_sprite_rec = (Rectangle){0, 0, sprite.width, sprite.height};
    ghost->ghost_rec = (Rectangle){x_posn, y_posn, TILE_SIZE, TILE_SIZE};
    ghost->ghost_origin = (Vector2){ghost->ghost_rec.width / 2, ghost->ghost_rec.height / 2};
    ghost->temp_target = (Vector2){0, 0};
    ghost->main_target = (Vector2){0, 0};
    ghost->start_posn = (Vector2){x_posn, y_posn};
    ghost->best_tile = -1;
    ghost->prev_tile = -1;
    ghost->ghost_direction = stop;
    ghost->ghost_speed = (Vector2){GHOST_BASE_SPEED, GHOST_BASE_SPEED};
    ghost->ghost_state = release_ghosthouse;
    ghost->prev_ghost_state = release_ghosthouse;
    ghost->is_frightened = 0;
}

int returnOppositeTile(int tile)
{

    if (tile != -1)
    {
        switch (tile)
        {
        case 0:
            return 2;
            break;

        case 1:
            return 3;
            break;

        case 2:
            return 0;
            break;

        case 3:
            return 1;
            break;
        }
    }

    return -1;
}

void getSprite(Ghost *ghost, Texture2D sprite_array[6])
{
    Texture2D buffer_sprite;

    if (ghost->ghost_state == frightened)
    {
        ghost->ghost_sprite = sprite_array[4];
        ghost->ghost_sprite_rec = (Rectangle){0, 0, sprite_array[4].width, sprite_array[4].height};
    }

    else if (ghost->ghost_state == back_to_ghosthouse)
    {

        ghost->ghost_sprite = sprite_array[5];
        ghost->ghost_sprite_rec = (Rectangle){0, 0, sprite_array[5].width, sprite_array[5].height};
    }

    else
    {

        int i = (int)ghost->name;

        ghost->ghost_sprite = sprite_array[i];
        ghost->ghost_sprite_rec = (Rectangle){0, 0, sprite_array[i].width, sprite_array[i].height};
    }
}

void getDistArray(Ghost *ghost, char maze[MAZE_HEIGHT][MAZE_WIDTH])
{
    int ghost_x = (int)(ghost->ghost_rec.x / TILE_SIZE);
    int ghost_y = (int)(ghost->ghost_rec.y / TILE_SIZE);

    Vector2 upper_tile = {(float)(ghost_x * TILE_SIZE) + TILE_SIZE / 2, (float)((ghost_y - 1) * TILE_SIZE) + TILE_SIZE / 2};
    Vector2 left_tile = {(float)((ghost_x - 1) * TILE_SIZE) + TILE_SIZE / 2, (float)(ghost_y * TILE_SIZE) + TILE_SIZE / 2};
    Vector2 lower_tile = {(float)(ghost_x * TILE_SIZE) + TILE_SIZE / 2, (float)((ghost_y + 1) * TILE_SIZE) + TILE_SIZE / 2};
    Vector2 right_tile = {(float)((ghost_x + 1) * TILE_SIZE) + TILE_SIZE / 2, (float)(ghost_y * TILE_SIZE) + TILE_SIZE / 2};

    Vector2 tile_array[4] = {upper_tile, left_tile, lower_tile, right_tile};

    int mazeI = 0;
    int mazeJ = 0;

    for (int i = 0; i < 4; i++)
    {
        distance_array[i] = DISTANCE_INFINITY;

        mazeI = (int)((tile_array[i].y) / TILE_SIZE);
        mazeJ = (int)(tile_array[i].x / TILE_SIZE);

        int ghost_gate = 0; // Variable to check if the condition is ghosts exiting from their house or not

        if (mazeI == 12 && mazeJ > 11 && mazeJ < 16)
        {
            if (ghost->ghost_state == release_ghosthouse || ghost->ghost_state == back_to_ghosthouse)
            {
                ghost_gate = 1; // If true, then invalidate the 'red' walls
            }
        }

        if (maze[mazeI][mazeJ] != '#' || ghost_gate)
        {
            distance_array[i] = Vector2Distance(tile_array[i], ghost->main_target);
        }
    }

    ghost->prev_tile = returnOppositeTile(ghost->best_tile);

    if (ghost->prev_tile != -1)
        distance_array[ghost->prev_tile] = DISTANCE_INFINITY;

    int viableMoves = 0;
    for (int i = 0; i < 4; i++)
    {
        if (distance_array[i] != DISTANCE_INFINITY)
        {
            viableMoves++;
        }
    }

    if (viableMoves == 0 && ghost->prev_tile != -1)
    {
        distance_array[ghost->prev_tile] = Vector2Distance(tile_array[ghost->prev_tile], ghost->main_target);
    }
}

// Moves ghost to a target point
void mazeMove(Ghost *ghost)
{
    // Bad teleport func
    if (ghost->ghost_rec.x >= 27 * TILE_SIZE - TILE_SIZE / 2 && floatCompare(ghost->ghost_rec.y, 15 * TILE_SIZE - TILE_SIZE / 2))
    {

        if (ghost->ghost_direction == move_left)
            ghost->ghost_rec.x -= GHOST_REDUCED_SPEED;
        if (ghost->ghost_direction == move_right)
            ghost->ghost_rec.x += GHOST_REDUCED_SPEED;
        if (ghost->ghost_rec.x >= 28 * TILE_SIZE - TILE_SIZE / 2 && ghost->ghost_direction == move_right)
        {
            ghost->ghost_rec.x = TILE_SIZE - TILE_SIZE / 2;
            ghost->ghost_direction = stop;
            return;
        }
        if (ghost->ghost_rec.x <= 28 * TILE_SIZE - TILE_SIZE / 2 && ghost->ghost_direction == move_left)
        {
            ghost->ghost_direction = stop;
        }

        return;
    }

    if (ghost->ghost_rec.x <= 2 * TILE_SIZE - TILE_SIZE / 2 && floatCompare(ghost->ghost_rec.y, 15 * TILE_SIZE - TILE_SIZE / 2))
    {
        if (ghost->ghost_direction == move_left)
            ghost->ghost_rec.x -= GHOST_REDUCED_SPEED;
        if (ghost->ghost_direction == move_right)
            ghost->ghost_rec.x += GHOST_REDUCED_SPEED;
        if (ghost->ghost_rec.x <= TILE_SIZE - TILE_SIZE / 2 && ghost->ghost_direction == move_left)
        {
            ghost->ghost_rec.x = 28 * TILE_SIZE - TILE_SIZE / 2;
            ghost->ghost_direction = stop;
            return;
        }
        if (ghost->ghost_rec.x >= TILE_SIZE - TILE_SIZE / 2 && ghost->ghost_direction == move_right)
        {
            ghost->ghost_direction = stop;
        }

        return;
    }

    if (ghost->ghost_state != frightened && ghost->ghost_state != back_to_ghosthouse && ghost->ghost_state != release_ghosthouse)
    {

        if ((int)ghost->ghost_rec.x % 2 != 0)
            ghost->ghost_speed.x /= 2;

        if ((int)ghost->ghost_rec.y % 2 != 0)
            ghost->ghost_speed.y /= 2;
    }

    if (ghost->ghost_state == back_to_ghosthouse)
    {
        if (!floatCompare(ghost->ghost_rec.x, ghost->start_posn.x) || !floatCompare(ghost->ghost_rec.y, ghost->start_posn.y))
        {
            if ((int)ghost->ghost_rec.x % 4 != 0)
            {
                if ((int)ghost->ghost_rec.x % 2 != 0)
                    ghost->ghost_speed.x /= 4;
                else if ((int)ghost->ghost_rec.x % 2 == 0)
                    ghost->ghost_speed.x /= 2;
            }

            if ((int)ghost->ghost_rec.y % 4 != 0)
            {
                if ((int)ghost->ghost_rec.y % 2 != 0)
                    ghost->ghost_speed.y /= 4;
                else if ((int)ghost->ghost_rec.y % 2 == 0)
                    ghost->ghost_speed.y /= 2;
            }
        }

        if (floatCompare(ghost->ghost_rec.x, ghost->main_target.x))
            ghost->ghost_speed.x /= 2;

        if (floatCompare(ghost->ghost_rec.y, ghost->main_target.y))
            ghost->ghost_speed.y /= 2;
    }

    if (!floatCompare(ghost->temp_target.x, ghost->ghost_rec.x))
        ghost->ghost_rec.x += ghost->ghost_speed.x;

    if (!floatCompare(ghost->temp_target.y, ghost->ghost_rec.y))
        ghost->ghost_rec.y += ghost->ghost_speed.y;

    if (floatCompare(ghost->temp_target.x, ghost->ghost_rec.x) && floatCompare(ghost->temp_target.y, ghost->ghost_rec.y))
    {
        ghost->ghost_direction = stop;
    }
}

// Trigger temp_target after ghost returns from frightened state
void moveGhost(Ghost *ghost, Vector2 speed)
{
    if (ghost->ghost_state == release_ghosthouse || ghost->ghost_state == frightened)
        speed = (Vector2){GHOST_REDUCED_SPEED, GHOST_REDUCED_SPEED};

    else if (ghost->ghost_state == back_to_ghosthouse)
        speed = (Vector2){GHOST_BACK_SPEED, GHOST_BACK_SPEED};

    if (ghost->best_tile == 0 || ghost->ghost_direction == move_up)
    {
        if (ghost->ghost_direction == stop)
        {
            ghost->temp_target.x = ghost->ghost_rec.x;
            ghost->temp_target.y = ghost->ghost_rec.y - TILE_SIZE;
            ghost->ghost_direction = move_up;
        }

        if (ghost->ghost_direction == move_up)
        {
            ghost->ghost_speed.y = -speed.y;
            ghost->ghost_speed.x = 0;
        }
    }

    else if (ghost->best_tile == 1 || ghost->ghost_direction == move_left)
    {
        if (ghost->ghost_direction == stop)
        {
            ghost->temp_target.x = ghost->ghost_rec.x - TILE_SIZE;
            ghost->temp_target.y = ghost->ghost_rec.y;
            ghost->ghost_direction = move_left;
        }

        if (ghost->ghost_direction == move_left)
        {
            ghost->ghost_speed.y = 0;
            ghost->ghost_speed.x = -speed.x;
        }
    }

    else if (ghost->best_tile == 2 || ghost->ghost_direction == move_down)
    {
        if (ghost->ghost_direction == stop)
        {
            ghost->temp_target.x = ghost->ghost_rec.x;
            ghost->temp_target.y = ghost->ghost_rec.y + TILE_SIZE;
            ghost->ghost_direction = move_down;
        }

        if (ghost->ghost_direction == move_down)
        {
            ghost->ghost_speed.y = speed.y;
            ghost->ghost_speed.x = 0;
        }
    }

    else if (ghost->best_tile == 3 || ghost->ghost_direction == move_right)
    {
        if (ghost->ghost_direction == stop)
        {
            ghost->temp_target.x = ghost->ghost_rec.x + TILE_SIZE;
            ghost->temp_target.y = ghost->ghost_rec.y;
            ghost->ghost_direction = move_right;
        }

        if (ghost->ghost_direction == move_right)
        {
            ghost->ghost_speed.y = 0;
            ghost->ghost_speed.x = speed.x;
        }
    }

    if ((int)ghost->temp_target.x % 2 != 0)
        ghost->temp_target.x = (int)(ghost->temp_target.x / TILE_SIZE) * TILE_SIZE + TILE_SIZE / 2;

    if ((int)ghost->temp_target.y % 2 != 0)
        ghost->temp_target.y = (int)(ghost->temp_target.y / TILE_SIZE) * TILE_SIZE + TILE_SIZE / 2;

    mazeMove(ghost);
}

int returnMinTile(float *distance_array)
{
    int minIndex = 0;

    for (int i = 0; i < 4; i++)
    {
        // Second condition indicates if two index values are equal, choose the smaller value
        if (distance_array[minIndex] > distance_array[i] || (floatCompare(distance_array[minIndex], distance_array[i]) && minIndex > i))
        {
            minIndex = i;
        }
    }

    return minIndex;
}

void moveToTarget(Ghost *ghost, char maze[MAZE_HEIGHT][MAZE_WIDTH], Vector2 ghost_speed, Ghost_State prev_ghosts_state)
{
    if (!floatCompare(ghost->ghost_rec.x, ghost->main_target.x) || !floatCompare(ghost->ghost_rec.y, ghost->main_target.y))
    {

        if (ghost->ghost_direction == stop)
        {
            getDistArray(ghost, maze);

            if (ghost->prev_ghost_state == scatter || ghost->prev_ghost_state == chase)
            {
                if (ghost->ghost_state != ghost->prev_ghost_state && ghost->prev_tile != -1) // Flip ghost's movement direction when it changes states
                {

                    distance_array[ghost->prev_tile] = 0;
                    for (int i = 0; i < 4; i++)
                    {
                        if (i != ghost->prev_tile)
                            distance_array[i] = DISTANCE_INFINITY;
                    }

                    ghost->prev_ghost_state = ghost->ghost_state;
                }
            }

            if (ghost->ghost_state == frightened)
            {
                ghost->best_tile = returnRandTile(distance_array);
            }
            else
            {
                ghost->best_tile = returnMinTile(distance_array);
            }
        }
    }

    else
    {

        if (ghost->prev_ghost_state == back_to_ghosthouse)
        {
            ghost->best_tile = 0;
            ghost->ghost_direction = stop;
        }
    }

    moveGhost(ghost, ghost_speed);
}

void setTarget(Ghost *ghost, Ghost ghosts[4], Player pacman, Ghost_State overall_ghosts_state)
{
    switch (ghost->ghost_state)
    {

    case in_ghosthouse:
    {

        for (int i = 0; i < 4; i++)
        {
            ghosts[i].main_target.x = ghosts[i].ghost_rec.x;
            ghosts[i].main_target.y = ghosts[i].ghost_rec.y;
        }
        break;
    }

    case release_ghosthouse:
    {
        ghost->main_target.x = ghost->start_posn.x - TILE_SIZE;
        ghost->main_target.y = ghost->start_posn.y - 2 * TILE_SIZE;
        if (floatCompare(ghost->ghost_rec.x, ghost->start_posn.x - TILE_SIZE) && floatCompare(ghost->ghost_rec.y, ghost->start_posn.y - 2 * TILE_SIZE))
        {
            ghost->ghost_state = overall_ghosts_state;
        }
        ghost->prev_ghost_state = release_ghosthouse;
        break;
    }

    case scatter:
    {

        switch (ghost->name)
        {

        case blinky:
        {

            ghost->main_target.x = 26 * TILE_SIZE;
            ghost->main_target.y = 2 * TILE_SIZE;
            break;
        }

        case inky:
        {
            ghost->main_target.x = 26 * TILE_SIZE;
            ghost->main_target.y = 30 * TILE_SIZE;
            break;
        }

        case pinky:
        {

            ghost->main_target.x = 2 * TILE_SIZE;
            ghost->main_target.y = 2 * TILE_SIZE;
            break;
        }

        case clyde:
        {
            ghost->main_target.x = 2 * TILE_SIZE;
            ghost->main_target.y = 30 * TILE_SIZE;
            break;
        }
        }
        break;
    }

    case chase:
    {
        switch (ghost->name)
        {

        case blinky:
        {
            ghost->main_target.x = pacman.pacman_rec.x - (float)(TILE_SIZE / 2);
            ghost->main_target.y = pacman.pacman_rec.y - (float)(TILE_SIZE / 2);
            break;
        }

        case inky:
        {
            Vector2 blinky_posn = {ghosts[0].ghost_rec.x, ghosts[0].ghost_rec.y};
            Vector2 temp_targ = {-TILE_SIZE / 2, -TILE_SIZE / 2};

            switch (pacman.prev_direction)
            {

            case move_up:
            {
                temp_targ.x += pacman.pacman_rec.x;
                temp_targ.y += pacman.pacman_rec.y - TILE_SIZE;

                break;
            }

            case move_left:
            {
                temp_targ.x += pacman.pacman_rec.x - TILE_SIZE;
                temp_targ.y += pacman.pacman_rec.y;

                break;
            }

            case move_down:
            {
                temp_targ.x += pacman.pacman_rec.x;
                temp_targ.y += pacman.pacman_rec.y + TILE_SIZE;

                break;
            }

            case move_right:
            {
                temp_targ.x += pacman.pacman_rec.x + TILE_SIZE;
                temp_targ.y += pacman.pacman_rec.y;

                break;
            }
            }

            ghost->main_target.x = blinky_posn.x + 2 * (temp_targ.x - blinky_posn.x);
            ghost->main_target.y = blinky_posn.y + 2 * (temp_targ.y - blinky_posn.y);
            break;
        }

        case pinky:
        {
            switch (pacman.prev_direction)
            {

            case move_up:
            {
                ghost->main_target.x = pacman.pacman_rec.x - TILE_SIZE / 2;
                ghost->main_target.y = pacman.pacman_rec.y - 2 * TILE_SIZE - TILE_SIZE / 2;
                break;
            }

            case move_left:
            {

                ghost->main_target.x = pacman.pacman_rec.x - 2 * TILE_SIZE - TILE_SIZE / 2;
                ghost->main_target.y = pacman.pacman_rec.y - TILE_SIZE / 2;
                break;
            }

            case move_down:
            {

                ghost->main_target.x = pacman.pacman_rec.x - TILE_SIZE / 2;
                ghost->main_target.y = pacman.pacman_rec.y + 2 * TILE_SIZE - TILE_SIZE / 2;
                break;
            }

            case move_right:
            {

                ghost->main_target.x = pacman.pacman_rec.x + 2 * TILE_SIZE - TILE_SIZE / 2;
                ghost->main_target.y = pacman.pacman_rec.y - TILE_SIZE / 2;
                break;
            }
            }
            break;
        }

        case clyde:
        {
            float clyde_to_pacman = Vector2Distance((Vector2){ghost->ghost_rec.x, ghost->ghost_rec.y}, (Vector2){pacman.pacman_rec.x, pacman.pacman_rec.y});

            if (clyde_to_pacman >= 4 * TILE_SIZE)
            {
                ghost->main_target.x = pacman.pacman_rec.x - TILE_SIZE / 2;
                ghost->main_target.y = pacman.pacman_rec.y - TILE_SIZE / 2;
            }

            else
            {
                ghost->main_target.x = 2 * TILE_SIZE;
                ghost->main_target.y = 30 * TILE_SIZE;
            }

            break;
        }
        }
        break;
    }

    case frightened:
    {
        break;
    }

    case back_to_ghosthouse:
    {
        static int flag = 0;
        if (!flag)
        {
            ghost->main_target.x = ghost->start_posn.x;
            ghost->main_target.y = ghost->start_posn.y - 2 * TILE_SIZE;
        }

        if (floatCompare(ghost->ghost_rec.x, ghost->start_posn.x) && floatCompare(ghost->ghost_rec.y, ghost->start_posn.y - 2 * TILE_SIZE))
        {

            ghost->main_target.x = ghost->start_posn.x;
            ghost->main_target.y = ghost->start_posn.y;
            flag = 1;
        }

        if (floatCompare(ghost->ghost_rec.x, ghost->start_posn.x) && floatCompare(ghost->ghost_rec.y, ghost->start_posn.y))
        {

            ghost->ghost_state = release_ghosthouse;
            ghost->prev_ghost_state = back_to_ghosthouse;
            flag = 0;
        }

        break;
    }
    }
}

int returnRandTile(float *distance_array)
{
    int rand_tile = rand() % 4;

    while (distance_array[rand_tile] == (float)DISTANCE_INFINITY)
    {

        rand_tile = rand() % 4;
    }

    return rand_tile;
}

void ghostCollision(Ghost *ghost, Player *pacman, Game_State *game_state)
{

    if ((int)(ghost->ghost_rec.x / TILE_SIZE) == (int)(pacman->pacman_rec.x / TILE_SIZE) && (int)(ghost->ghost_rec.y / TILE_SIZE) == (int)(pacman->pacman_rec.y / TILE_SIZE))
    {
        if (ghost->ghost_state != frightened && ghost->ghost_state != back_to_ghosthouse)
        {
            if (*game_state == GAME_RUNNING)
            {
                if (pacman->life_count != 0)
                {
                    pacman->life_count--;
                    *game_state = LIFE_LOST;
                }
                if (pacman->life_count == 0)
                {

                    *game_state = GAME_OVER;
                }
            }
        }

        if (ghost->ghost_state == frightened)
        {
            ghost->ghost_state = back_to_ghosthouse;
            pacman->score += 200;
        }
    }
}
