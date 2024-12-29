#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "gamedefs.h"
#include "gamefuncdefs.h"
#include "pac.h"
#include "ghost.h"
#include "pellets.h"

int floatCompare(float f1, float f2)
{

    float precision = 0.0001;

    if (f1 - precision < f2 && f1 + precision > f2)
        return 1;

    else
        return 0;
}

void drawGridLines()
{
    Vector2 lineH = {0, 0};
    Vector2 lineV = {0, 0};

    for (int i = 1; i < MAZE_HEIGHT; i++)
    {
        lineH.y = i * TILE_SIZE;

        DrawLine(lineH.x, lineH.y, MAZE_WIDTH * TILE_SIZE, lineH.y, RED);
    }

    for (int j = 1; j < MAZE_WIDTH; j++)
    {

        lineV.x = j * TILE_SIZE;
        DrawLine(lineV.x, lineV.y, lineV.x, MAZE_HEIGHT * TILE_SIZE, RED);
    }
}

void drawMaze(char maze[MAZE_HEIGHT][MAZE_WIDTH])
{

    Rectangle wall = {0, 0, TILE_SIZE, TILE_SIZE};

    for (int i = 0; i < MAZE_HEIGHT; i++)
    {

        for (int j = 0; j < MAZE_WIDTH; j++) // Walls or ghosthouse gates
        {

            if (maze[i][j] == '#')
            {

                if (i == 12 && j > 11 && j < 16) // i = 12 and j = 12 to 15, draw red for the gate
                {
                    DrawRectangleRec((Rectangle){j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE / 2}, RED);
                }
                else
                {
                    wall.x = j * TILE_SIZE;
                    wall.y = i * TILE_SIZE;

                    DrawRectangleLinesEx(wall, 3, DARKBLUE);
                    // DrawRectangleRec(wall, RAYWHITE);
                }
            }

            if (maze[i][j] == '.') // Normal pellets
            {

                DrawCircle((j * TILE_SIZE) + TILE_SIZE / 2, (i * TILE_SIZE) + TILE_SIZE / 2, 4, RAYWHITE);
            }

            if (maze[i][j] == 'O') // Draw power pellets
            {

                DrawCircle((j * TILE_SIZE) + TILE_SIZE / 2, (i * TILE_SIZE) + TILE_SIZE / 2, 12, RAYWHITE);
            }
        }
    }
}

void resetGame(Ghost ghosts[4], Texture2D ghost_sprite_array[6], Player *pacman, Texture2D pacman_narrow)
{

    for (int i = 0; i < 4; i++)
    {
        initGhost(&ghosts[i], ghost_sprite_array[i], ghosts[i].start_posn.x, ghosts[i].start_posn.y, ghosts[i].name);
    }

    initPacman(pacman, pacman_narrow, pacman->score, pacman->life_count);
}

int main()
{
    const int screen_height = MAZE_HEIGHT * TILE_SIZE;
    const int screen_width = MAZE_WIDTH * TILE_SIZE + 10 * TILE_SIZE; // Plus 10 Tiles to house the score, username and lives

    InitWindow(screen_width, screen_height, "PACMAN");

    Texture2D pacman_narrow = LoadTexture("Resources/PacmanSprites/pacNarrow.png");
    Texture2D pacman_wide = LoadTexture("Resources/PacmanSprites/pacWide.png");

    Player pacman;

    initPacman(&pacman, pacman_narrow, 0, 3);

    Texture2D blinky_sprite = LoadTexture("Resources/GhostSprites/blinky.png");
    Texture2D inky_sprite = LoadTexture("Resources/GhostSprites/inky.png");
    Texture2D pinky_sprite = LoadTexture("Resources/GhostSprites/pinky.png");
    Texture2D clyde_sprite = LoadTexture("Resources/GhostSprites/clyde.png");
    Texture2D frightened_ghost = LoadTexture("Resources/GhostSprites/frightened.png");
    Texture2D ghost_eyes = LoadTexture("Resources/GhostSprites/frightened_transition.png");

    Texture2D sprite_array[6] = {blinky_sprite, inky_sprite, pinky_sprite, clyde_sprite, frightened_ghost, ghost_eyes};

    Ghost ghost_blinky, ghost_inky, ghost_pinky, ghost_clyde;

    initGhost(&ghost_blinky, blinky_sprite, 13 * TILE_SIZE - 16, 14 * TILE_SIZE - 16, blinky);
    initGhost(&ghost_inky, inky_sprite, 15 * TILE_SIZE - 16, 14 * TILE_SIZE - 16, inky);
    initGhost(&ghost_pinky, pinky_sprite, 14 * TILE_SIZE - 16, 14 * TILE_SIZE - 16, pinky);
    initGhost(&ghost_clyde, clyde_sprite, 16 * TILE_SIZE - 16, 14 * TILE_SIZE - 16, clyde);

    Ghost ghosts[4] = {ghost_blinky, ghost_inky, ghost_pinky, ghost_clyde};
    Ghost_State overall_ghosts_state = release_ghosthouse;
    Ghost_State prev_ghosts_state = release_ghosthouse;
    int is_frightened = 0;

    ghosts[0].ghost_state = release_ghosthouse;
    ghosts[1].ghost_state = release_ghosthouse;
    ghosts[2].ghost_state = release_ghosthouse;
    ghosts[3].ghost_state = release_ghosthouse;

    Vector2 ghost_speed = {GHOST_BASE_SPEED, GHOST_BASE_SPEED};

    Font score_font = LoadFont("Resources/Font/pacfont.ttf");

    char maze[MAZE_HEIGHT][MAZE_WIDTH] = {
        "############################",
        "#............##............#",
        "#.####.#####.##.#####.####.#",
        "#O####.#####.##.#####.####O#",
        "#.####.#####.##.#####.####.#",
        "#..........................#",
        "#.####.##.########.##.####.#",
        "#.####.##.########.##.####.#",
        "#......##....##....##......#",
        "######.##### ## #####.######",
        "     #.##### ## #####.#     ",
        "     #.##          ##.#     ",
        "     #.## ######## ##.#     ",
        "######.## #      # ##.######",
        "      .   ########   .      ",
        "######.## ######## ##.######",
        "     #.## ######## ##.#     ",
        "     #.##          ##.#     ",
        "     #.## ######## ##.#     ",
        "######.## ######## ##.######",
        "#............##............#",
        "#.####.#####.##.#####.####.#",
        "#.####.#####.##.#####.####.#",
        "#...##................##...#",
        "###.##.##.########.##.##.###",
        "###.##.##.########.##.##.###",
        "#......##....##....##......#",
        "#.##########.##.##########.#",
        "#.##########.##.##########.#",
        "#O.......................O.#",
        "############################"};

    int frame_counter = 0;
    SetTargetFPS(FRAMERATE);

    Game_State game_state = USER; // Set to USER for normal function
    Game_State prev_game_state = USER;
    char user_name[10] = "\0"; // "\0"
    char *disp_text = user_name;
    char game_res = '\0';
    int letterCount = 0;
    int write_high_score = 0;
    int game_ended = 0;

    time_t start_time = time(NULL);
    time_t frightened_start = 0;
    time_t current_time = time(NULL);

    int state_timer = 0;
    int frightened_timer = 0;
    int is_frightened = 0;

    while (!WindowShouldClose())
    {
        printf("%d-->%d AND %d\n", state_timer, overall_ghosts_state, frightened_timer);

        switch (game_state)
        {

        case USER:
            if (IsKeyPressed(KEY_ENTER))
            {
                game_state = READY;
                prev_game_state = LIFE_LOST;
            }
            break;

        case READY:
        {
            if (prev_game_state == LIFE_LOST)
            {
                resetGame(ghosts, sprite_array, &pacman, pacman_narrow);
                start_time = current_time;
                state_timer = 0;
                prev_game_state = READY;
            }

            if (state_timer == 3)
            {
                game_state = GAME_RUNNING;
            }
            break;
        }

        case LIFE_LOST:
        {
            sleep(2);
            prev_game_state = LIFE_LOST;
            game_state = READY;
            break;
        }

        case GAME_RUNNING:
            break;
        case GAME_OVER:
            break;
        case WIN:
            break;

        case HIGH_SCORE:
        {
            if (!write_high_score)
            {
                FILE *file_ptr;

                file_ptr = fopen("highscores.txt", "r");

                struct Ind_Buf
                {
                    char buf[50];
                    int numbuf;
                } Ind_Buf;

                struct Ind_Buf score_arr[10] = {0}; // Maximum of 10 entries can be displayed

                int ind = 0;

                char temp_ch = '\0';

                while (fscanf(file_ptr, "%[a-zA-Z0-9] %c %d\n", score_arr[ind].buf, &temp_ch, &(score_arr[ind].numbuf)) == 3 && ind < 10)
                {
                    sprintf(score_arr[ind].buf, "%s %c ", score_arr[ind].buf, temp_ch);
                    ind++;
                }

                fclose(file_ptr);

                int mode = 0; // decides the operation to be done

                int current_score_written = 0;

                if (ind > 0 && pacman.score > score_arr[ind - 1].numbuf) // if score is greater than smallest available score
                {
                    mode = 1;
                }

                else if (ind < 10) // if score is lesser than any of the available scores or nothing is in the file
                {
                    mode = 1;

                    sprintf(score_arr[ind].buf, "%s %c ", user_name, game_res);

                    score_arr[ind].numbuf = pacman.score;

                    current_score_written = 1;
                    ind++;
                }

                if (mode == 1)
                {
                    file_ptr = fopen("highscores.txt", "w");

                    for (int i = 0; i < ind; i++)
                    {
                        if (pacman.score >= score_arr[i].numbuf && current_score_written == 0)
                        {
                            int bound = ind;

                            if (ind < 10)
                            {
                                ind++;
                            }
                            else
                            {
                                bound--;
                            }

                            for (int j = bound; j > i; j--)
                            {
                                strcpy(score_arr[j].buf, score_arr[j - 1].buf);
                                score_arr[j].numbuf = score_arr[j - 1].numbuf;
                            }

                            sprintf(score_arr[i].buf, "%s %c ", user_name, game_res);

                            score_arr[i].numbuf = pacman.score;

                            current_score_written = 1;
                        }

                        fprintf(file_ptr, "%s%d\n", score_arr[i].buf, score_arr[i].numbuf);
                    }

                    fclose(file_ptr);
                }

                write_high_score = 1;
            }
            break;
        }
        }

        if (game_state != USER && game_state != HIGH_SCORE)
        {
            current_time = time(NULL);
            state_timer = current_time - start_time;
        }

        if (game_state != GAME_OVER && game_state != WIN && game_state != HIGH_SCORE)
        {
            for (int i = 0; i < 4; i++)
            {
                ghostCollision(&ghosts[i], &pacman, &game_state);
            }

            if (pelletCount(maze) == 0)
            {
                game_state = WIN;
            }

            if (overall_ghosts_state == frightened)
            {

                if (prev_ghosts_state != frightened)
                {
                    frightened_start = current_time;
                }

                time_t current_frightened = time(NULL);
                frightened_timer = current_frightened - frightened_start;

                if (frightened_timer > FRIGHTENED_TIME)
                {
                    overall_ghosts_state = scatter;
                    frightened_timer = 0;
                }
            }

            if (overall_ghosts_state != frightened)
            {
                if (state_timer >= 0 && state_timer < SCATTER_TIME)
                {
                    overall_ghosts_state = scatter;
                }

                if (state_timer >= SCATTER_TIME && state_timer < CHASE_TIME)
                {
                    overall_ghosts_state = chase;
                }
            }

            if (state_timer >= CHASE_TIME)
            {
                state_timer = 0;
                start_time = current_time;
            }

            for (int i = 0; i < 4; i++)
            {

                if (ghosts[i].ghost_state != frightened)
                {
                    if (ghosts[i].ghost_state != release_ghosthouse && ghosts[i].ghost_state != back_to_ghosthouse)
                    {
                        ghosts[i].ghost_state = overall_ghosts_state;
                    }
                }
                else
                {
                    if (ghosts[i].prev_ghost_state == release_ghosthouse)
                    {
                        ghosts[i].ghost_state = overall_ghosts_state;
                    }
                }
            }

            getPacmanSprite(&pacman, pacman_wide, pacman_narrow, 4);

            if (game_state != READY && game_state != LIFE_LOST)
                pacMove(&pacman, maze);

            for (int i = 0; i < 4; i++)
            {
                setTarget(&ghosts[i], ghosts, pacman, overall_ghosts_state);
                moveToTarget(&ghosts[i], maze, ghost_speed, prev_ghosts_state);
            }

            pelletCollision(&pacman, maze, 10);
            for (int i = 0; i < 4; i++)
            {
                if (prev_ghosts_state != overall_ghosts_state)
                    if (ghosts[i].ghost_state != back_to_ghosthouse)
                        ghosts[i].prev_ghost_state = prev_ghosts_state;
            }

            prev_ghosts_state = overall_ghosts_state;
            powPel(&pacman, &overall_ghosts_state, maze, 200);
        }

        BeginDrawing();

        ClearBackground(BLACK);

        if (game_state == USER)
        {
            Rectangle textBox = {screen_width / 2.0f - 256, screen_height / 2.0f - 128, 512, 64};

            int key = GetCharPressed();

            while (key > 0)
            {
                if (letterCount < 10)
                {
                    if ((key >= 32) && (key <= 125))
                    {
                        user_name[letterCount] = (char)key;
                        letterCount++;
                    }
                }

                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                if (letterCount > 0)
                    letterCount--;
                user_name[letterCount] = '\0';
            }

            if (IsKeyPressed(KEY_ENTER))
            {
                user_name[letterCount] = '\0';
            }

            DrawRectangleRec(textBox, RAYWHITE);
            DrawTextEx(score_font, "ENTER USER NAME", (Vector2){(int)textBox.x + 4, (int)textBox.y - 64}, 40, 2, RAYWHITE);
            DrawTextEx(score_font, user_name, (Vector2){(int)textBox.x + 5, (int)textBox.y + 8}, 40, 2, BLACK);
        }

        if (game_state == HIGH_SCORE)
        {
            static int flag = 0; // First iteration stores everything in buf from the file, and flag is triggered, after which everything is rendered from buf

            FILE *filep;

            if (flag == 0)
            {
                filep = fopen("highscores.txt", "r");
            }

            char buf[10][50];
            static int ind = 0; // Counts num of elements in file, in the first iteration
            int i = 0;

            DrawTextEx(score_font, "HIGH SCORES", (Vector2){(screen_width / 2) - 256 - 40, 40}, 64, 5, RAYWHITE);

            if (flag == 0)
            {
                while (fscanf(filep, "%[^\n]\n", buf[ind]) == 1)
                {
                    ind++;
                }
            }

            while (i < ind)
            {
                char res;

                sscanf(buf[i], "%*[a-zA-A0-9] %c %*d\n", &res);

                Color col = res == 'W' ? (Color){0, 255, 0, 255} : (Color){255, 0, 0, 255};

                DrawTextEx(score_font, TextFormat("%d . %s", i + 1, buf[i]), (Vector2){(screen_width / 2) - 256 - 40 - (32 * (ind == 10)), 80 * (i + 2)}, 32, 5, col);

                i++;
            }

            if (flag == 0)
            {
                flag = 1;
                fclose(filep);
            }
        }

        if (game_state != USER && game_state != HIGH_SCORE)
        {
            drawMaze(maze);
            DrawTexturePro(pacman.pacman_sprite, pacman.pacman_sprite_rec, pacman.pacman_rec, pacman.pacman_origin, pacman.pacman_rotation, WHITE);

            for (int i = 0; i < 4; i++)
            {
                getSprite(&ghosts[i], sprite_array);
                DrawTexturePro(ghosts[i].ghost_sprite, ghosts[i].ghost_sprite_rec, ghosts[i].ghost_rec, ghosts[i].ghost_origin, 0.0f, WHITE);
            }

            if (game_state == READY)
            {
                if (state_timer < 2)
                    DrawTextEx(score_font, "READY?", (Vector2){(screen_width / 2) - 256 - 48, screen_height / 2 - 32}, 64, 5, RAYWHITE);
                if (state_timer >= 2)
                    DrawTextEx(score_font, "GO!", (Vector2){(screen_width / 2) - 256 + 24, screen_height / 2 - 32}, 64, 5, RAYWHITE);
            }

            if (game_state == GAME_OVER)
            {
                DrawTextEx(score_font, "GAME OVER", (Vector2){(screen_width / 2) - 256 - 40, screen_height / 2 - 16}, 32, 5, (Color){255, 0, 0, 255});

                if (!game_ended)
                {
                    start_time = current_time;
                    state_timer = 0;
                    game_ended = 1;
                }

                if (state_timer >= 5)
                {
                    game_res = 'L';
                    game_state = HIGH_SCORE;
                }
            }

            if (game_state == WIN)
            {
                DrawTextEx(score_font, "YOU WIN!", (Vector2){(screen_width / 2) - 256 - 24, screen_height / 2 - 16}, 32, 5, (Color){0, 255, 0, 255});

                if (!game_ended)
                {
                    start_time = current_time;
                    state_timer = 0;
                    game_ended = 1;
                }

                if (state_timer >= 5)
                {
                    game_res = 'W';
                    game_state = HIGH_SCORE;
                }
            }

            DrawTextEx(score_font, TextFormat("Lives:%i", pacman.life_count), (Vector2){screen_width - 304, 64}, 20, 5, RAYWHITE);

            DrawTextEx(score_font, TextFormat("Score:%i", pacman.score), (Vector2){screen_width - 304, screen_height - 64}, 20, 5, RAYWHITE);

            DrawTextEx(score_font, "Player:", (Vector2){screen_width - 304, screen_height / 2 - 22}, 20, 5, RAYWHITE);
            DrawTextEx(score_font, TextFormat("  %s", user_name), (Vector2){screen_width - 304, screen_height / 2}, 20, 5, RAYWHITE);
        }

        EndDrawing();
    }

    UnloadTexture(pacman_narrow);
    UnloadTexture(pacman_wide);

    UnloadTexture(blinky_sprite);
    UnloadTexture(inky_sprite);
    UnloadTexture(pinky_sprite);
    UnloadTexture(clyde_sprite);
    UnloadTexture(frightened_ghost);
    UnloadTexture(ghost_eyes);

    UnloadFont(score_font);

    CloseWindow();
}