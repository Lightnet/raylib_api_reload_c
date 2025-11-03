#include <raylib.h>
#include "loader.h"
#include <stddef.h>

int main(void) {
    InitWindow(800, 600, "Raylib Hot Reload Example");
    SetTargetFPS(60);

    LoadGame();
    SetGameRaylibFunctions();  // Set pointers after loading
    if (InitGame != NULL) InitGame();

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_R)) {
            UnloadGame();
            LoadGame();
            SetGameRaylibFunctions();  // Re-set pointers after reload
            if (InitGame != NULL) InitGame();  // Re-initialize after reload
        }

        UpdateGame();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawGame();
        EndDrawing();
    }

    UnloadGame();
    CloseWindow();
    return 0;
}