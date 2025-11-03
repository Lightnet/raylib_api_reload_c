#include "loader.h"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOSHOWWINDOW
#define NOUSER
#define NOWH
#include <windows.h>
#endif
#include <raylib.h>  // For raylib function addresses

static HMODULE hGame = NULL;

void (*InitGame)(void) = NULL;
void (*UpdateGame)(void) = NULL;
void (*DrawGame)(void) = NULL;

typedef void (*SetRaylibFuncsPtr)(
    void (*traceLog)(int logType, const char *text, ...),
    void (*drawText)(const char *text, int posX, int posY, int fontSize, Color color),
    void (*drawCircle)(int centerX, int centerY, float radius, Color color),
    int (*getScreenWidth)(void),
    int (*getScreenHeight)(void)
);

void LoadGame(void) {
    // Copy the built DLL to a temp file to avoid locking issues during rebuilds
    CopyFileA("game.dll", "game_temp.dll", FALSE);

    hGame = LoadLibraryA("game_temp.dll");
    if (hGame == NULL) {
        TraceLog(LOG_ERROR, "Failed to load game_temp.dll");
        return;
    }

    InitGame = (void(*)(void)) GetProcAddress(hGame, "InitGame");
    UpdateGame = (void(*)(void)) GetProcAddress(hGame, "UpdateGame");
    DrawGame = (void(*)(void)) GetProcAddress(hGame, "DrawGame");

    if (InitGame == NULL || UpdateGame == NULL || DrawGame == NULL) {
        TraceLog(LOG_ERROR, "Failed to get function addresses from DLL");
    }
}

void SetGameRaylibFunctions(void) {
    if (hGame == NULL) return;

    SetRaylibFuncsPtr setFuncs = (SetRaylibFuncsPtr) GetProcAddress(hGame, "SetRaylibFunctions");
    if (setFuncs == NULL) {
        TraceLog(LOG_ERROR, "Failed to get SetRaylibFunctions from DLL");
        return;
    }

    setFuncs(
        &TraceLog,
        &DrawText,
        &DrawCircle,
        &GetScreenWidth,
        &GetScreenHeight
    );
}

void UnloadGame(void) {
    if (hGame != NULL) {
        FreeLibrary(hGame);
        hGame = NULL;
        InitGame = NULL;
        UpdateGame = NULL;
        DrawGame = NULL;
    }
}