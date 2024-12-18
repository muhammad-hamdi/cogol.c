#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "raylib.h"

/*
    Any live cell with fewer than two live neighbours dies, as if by underpopulation.
    Any live cell with two or three live neighbours lives on to the next generation.
    Any live cell with more than three live neighbours dies, as if by overpopulation.
    Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
*/

typedef bool cell;

typedef enum Mode {
    EDIT,
    RUN
} Mode;

int check_neighbors(cell *world, int i, int size, int stride) {
    // check 8 neigbors and handle edge cases
    // TODO: do this more efficiently somehow
    int acc = 0;
    if(i > 0) {
        if(world[i-1]) acc++;
    }
    if(i + 1 < size) {
        if(world[i+1]) acc++;
    }
    if(i >= stride) {
        if(world[i-stride]) acc++;
    }
    if(i >= stride + 1) {
        if(world[i-stride-1]) acc++;
    }
    if(i >= stride - 1) {
        if(world[i-stride+1]) acc++;
    }
    if(i + stride < size) {
        if(world[i+stride]) acc++;
    }
    if(i + stride + 1 < size) {
        if(world[i+stride+1]) acc++;
    }
    if(i + stride - 1 < size) {
        if(world[i+stride-1]) acc++;
    }
    return acc;
}

int main() {
    int screen_width = 800;
    int screen_height = 600;
    InitWindow(screen_width, screen_height, "Cogol");

    int cell_size = 10;
    const int cell_count = screen_width/cell_size * screen_height/cell_size;

    cell *world = calloc(cell_count, sizeof(cell));

    cell *back_world = calloc(cell_count, sizeof(cell));

    int stride = screen_width/cell_size;

    Mode mode = EDIT;

    SetTargetFPS(60);
    int delay = 0;

    while(!WindowShouldClose()) {
        if(IsKeyDown(KEY_E)) {
            mode = EDIT;
        }
        if(IsKeyDown(KEY_R)) {
            mode = RUN;
        }
        BeginDrawing();
            ClearBackground(RAYWHITE);
            for (int i = 0; i < cell_count; i++)
            {
                int x = (i%stride)*cell_size;
                int y = (i/stride)*cell_size + 40;

                if(mode == RUN) {
                    int neigbor_count = check_neighbors(world, i, cell_count, stride);
                    back_world[i] = neigbor_count == 3 || (world[i] && neigbor_count == 2);
                }

                if(mode == EDIT) {
                    Vector2 m_pos = GetMousePosition();
                    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                        if(m_pos.x > x && m_pos.x < x+cell_size && m_pos.y > y && m_pos.y < y+cell_size) {
                            world[i] = true;
                        }
                    }
                    if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                        if(m_pos.x > x && m_pos.x < x+cell_size && m_pos.y > y && m_pos.y < y+cell_size) {
                            world[i] = false;
                        }
                    }

                    if(IsKeyDown(KEY_C)) {
                        memset(world, false, cell_count);
                    }
                }

                DrawRectangle(x, y, cell_size, cell_size, world[i] ? WHITE : BLACK);

                if(mode == EDIT) {
                    DrawText("Edit Mode", 20, 10, 20, RED);

                    Vector2 m_pos = GetMousePosition();
                    if(m_pos.x > x && m_pos.x < x+cell_size && m_pos.y > y && m_pos.y < y+cell_size) {
                        DrawRectangle(x, y, cell_size, cell_size, GRAY);
                    }
                }
                if(mode == RUN) {
                    DrawText("Running", 20, 10, 20, RED);
                }
            }
            if(mode == RUN) delay++;
            if(mode == RUN && delay == 10) {
                cell *tmp = world;
                world = back_world;
                back_world = tmp;
                memset(back_world, false, cell_count);
                delay = 0;
            }
        EndDrawing();
    }

    free(world);
    free(back_world);

    CloseWindow();

    return 0;
}