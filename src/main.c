// #define HOT_RELOAD
#include "game.h"
#include <windows.h>
#include <stdio.h>

typedef void (*GameInitProc)(void);
typedef void (*GameUpdateProc)(void);
typedef int (*GetReloadProc)(void);
typedef void (*GameCleanupProc)(void);
typedef int (*GetCloseProc)(void);

HINSTANCE dll_handle = NULL;
GameInitProc game_init;
GameUpdateProc game_update;
GetReloadProc get_reload;
GameCleanupProc game_cleanup;
GetCloseProc get_should_close;

void LoadDLL(void) {
    CopyFile("game.dll", "game_temp.dll", FALSE);
    dll_handle = LoadLibrary("game_temp.dll");
    if (!dll_handle) {
        printf("Failed to load DLL! Error: %lu\n", GetLastError());
        return;
    }

    game_init = (GameInitProc)GetProcAddress(dll_handle, "GameInit");
    game_update = (GameUpdateProc)GetProcAddress(dll_handle, "GameUpdate");
    get_reload = (GetReloadProc)GetProcAddress(dll_handle, "GetReloadFlag");
    get_should_close = (GetCloseProc)GetProcAddress(dll_handle, "GetShouldClose");
    game_cleanup = (GameCleanupProc)GetProcAddress(dll_handle, "GameCleanup");

    if (game_init && game_update && get_reload && get_should_close) {
        game_init();  // DLL creates window
        printf("[Main Loader] DLL loaded and initialized\n");
    } else {
        printf("[Main Loader] Failed to get DLL functions!\n");
    }
}

void UnloadDLL(void) {
    if (dll_handle) {
        if (game_cleanup) game_cleanup();
        // DLL should clean up? But CloseWindow in update loop.
        FreeLibrary(dll_handle);
        dll_handle = NULL;
        DeleteFile("game_temp.dll");
        printf("[Main Loader] DLL unloaded\n");
    }
}

int main(void) {
    LoadDLL();  // Initial load → DLL inits window

    int running = 1;
    while (running) {
        if (dll_handle && game_update) {
            game_update();  // DLL handles loop, drawing, close check

            // If DLL window closed, exit
            // (Can't query WindowShouldClose directly, so check if update "ran")
            // Or add a GetShouldClose() func to DLL.

            if (get_reload && get_reload()) {
                UnloadDLL();
                printf("[Main Loader] Press Enter after rebuilding game.dll...\n");
                // getchar();// terminal press enter
                LoadDLL();
            }

            if (get_should_close && get_should_close()) {
                printf("[Main Loader] Window closed - exiting\n");
                running = 0;
            }

            
        } else {
            // No DLL: Sleep to avoid CPU spin
            Sleep(100);
            running = 0;  // Or retry load
        }

        // Global exit if no window (e.g., ESC or manual close)
        // For multi-instance, each main.exe is independent.
    }

    UnloadDLL();
    printf("[Main Loader] Exiting instance\n");
    return 0;
}