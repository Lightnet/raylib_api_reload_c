// #define HOT_RELOAD
#include "raylib.h"
#include "game.h"
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOSHOWWINDOW
#define NOUSER
#define NOWH
#include <windows.h>
#endif
#include <stdio.h>

typedef void (*LoadAPIProc)(const RaylibAPI*);
typedef void (*GameInitProc)(void);
typedef void (*GameUpdateProc)(void);
typedef int (*GetReloadProc)(void);
typedef int (*GetVersionProc)(void);

HINSTANCE dll_handle = NULL;
LoadAPIProc load_api;
GameInitProc game_init;
GameUpdateProc game_update;
GetReloadProc get_reload;
GetVersionProc get_version;
int current_version = 0;

void LoadDLL(void) {
    // Shadow copy to allow rebuild (copy to temp DLL)
    CopyFile("game.dll", "game_temp.dll", FALSE);
    dll_handle = LoadLibrary("game_temp.dll");
    if (!dll_handle) {
        printf("Failed to load DLL!\n");
        return;
    }

    load_api = (LoadAPIProc)GetProcAddress(dll_handle, "LoadRaylibAPI");
    game_init = (GameInitProc)GetProcAddress(dll_handle, "GameInit");
    game_update = (GameUpdateProc)GetProcAddress(dll_handle, "GameUpdate");
    get_reload = (GetReloadProc)GetProcAddress(dll_handle, "GetReloadFlag");
    get_version = (GetVersionProc)GetProcAddress(dll_handle, "GetGameVersion");

    if (load_api && game_init && game_update && get_reload) {
        RaylibAPI api = {
            InitWindow, CloseWindow, WindowShouldClose,
            BeginDrawing, DrawText, EndDrawing, ClearBackground,
            GetKeyPressed
        };
        load_api(&api);
        game_init();
        current_version = get_version ? get_version() : 0;
        printf("[Main] DLL loaded (version %d)\n", current_version);
    }
}

void UnloadDLL(void) {
    if (dll_handle) {
        FreeLibrary(dll_handle);
        dll_handle = NULL;
        DeleteFile("game_temp.dll");  // Clean up shadow
    }
}

int main(void) {
    InitWindow(800, 600, "raylib Hot Reload - Press R");
    SetTargetFPS(60);

    LoadDLL();  // Initial load

    while (!WindowShouldClose()) {
        if (dll_handle && game_update) {
            game_update();

            // Check reload flag
            if (get_reload && get_reload()) {
                UnloadDLL();
                printf("[Main] Press Enter after rebuilding game.dll...\n");
                getchar();  // Wait for Enter
                LoadDLL();  // Reload
            }
        } else {
            // Fallback if no DLL
            BeginDrawing();
            ClearBackground(RED);
            DrawText("DLL not loaded!", 10, 10, 20, WHITE);
            EndDrawing();
        }
    }

    UnloadDLL();
    CloseWindow();
    return 0;
}