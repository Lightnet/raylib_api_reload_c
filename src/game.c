// #define HOT_RELOAD
#include "raylib.h"
#include <stdio.h>

static int reload_flag = 0;
static int initialized = 0;  // Guard: Init only once per process
static int should_close = 0;  // Reset on reload if needed

void GameInit(void) {
    if (initialized) {
        printf("[Game DLL] Already initialized - skipping InitWindow on reload\n");
        return;
    }
    InitWindow(800, 600, "raylib Hot Reload Instance - Press R");
    SetTargetFPS(60);
    initialized = 1;
    printf("[Game DLL] Window initialized (first time only)\n");
}

void GameUpdate(void) {
    // Raylib auto-skips if no window - but with guard, window persists
    if (!IsWindowReady()) {
        // Fallback if somehow lost
        printf("[Game DLL] Window lost? Re-init not allowed.\n");
        return;
    }

    if (WindowShouldClose()) {
        // Don't close here - let main handle exit
        should_close = 1;  // Or add separate close flag
        printf("[Game DLL] Close requested - flagging for exit\n");
        return;
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawText("Hot Reload Me! Guarded Init - No Re-Init Errors.", 10, 10, 20, DARKGRAY);
    DrawText("Press R to reload (window stays open).", 10, 40, 20, BLUE);
    DrawFPS(10, 70);
    // DrawText("Test Update.", 10, 90, 20, BLUE);// test change

    if (IsKeyPressed(KEY_R)) {
        reload_flag = 1;
        printf("[Game DLL] Reload triggered! Rebuild + Enter.\n");
    }

    EndDrawing();
}

int GetReloadFlag(void) {
    int flag = reload_flag;
    reload_flag = 0;
    return flag;
}

int GetShouldClose(void) {
    return should_close;
}

// Close guard: Uninit only on final unload (optional)
// GAME_API void GameCleanup(void);  // Add to game.h
void GameCleanup(void) {
    if (initialized && IsWindowReady()) {
        CloseWindow();
        initialized = 0;
        printf("[Game DLL] Cleaned up window\n");
    }
}