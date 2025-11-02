// #define HOT_RELOAD
#include "game.h"
#include <stdio.h>

static RaylibAPI api;
static int reload_flag = 0;

void LoadRaylibAPI(const RaylibAPI *in_api) {
    api = *in_api;
}

void GameInit(void) {
    // No InitWindow here—main handles it
    printf("[Game] Initialized\n");
}

void GameUpdate(void) {
    api.BeginDrawing();
    api.ClearBackground(RAYWHITE);
    // api.ClearBackground(BLUE);
    // BeginDrawing();

    // Example drawing (hot-reload this!)
    // DrawText("Hot Reload Me! Press R to reload.", 10, 10, 20, DARKGRAY);
    api.DrawText("Hello Window!", 10, 10, 20, DARKGRAY);
    api.DrawText("Hot Reload Me! Press R to reload!", 10, 30, 20, DARKGRAY);
    // printf("[Game] render...\n");

    // Check for 'R' press (KEY_R = 82)
    int key = api.GetKeyPressed();
    if (key == 82) {  // 'R'
        reload_flag = 1;
        printf("[Game] Reload triggered! Rebuild game.dll and press Enter in terminal.\n");
    }

    api.EndDrawing();
    // EndDrawing();
}

int GetReloadFlag(void) {
    return reload_flag;
}

// Export version info (optional, for reload detection)
int GetGameVersion(void) { return 1; }  // Bump on changes