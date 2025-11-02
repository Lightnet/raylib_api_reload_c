#pragma once
#include "raylib.h"

#if defined(_WIN32)
    #if defined(BUILD_DLL)
        #define GAME_API __declspec(dllexport)
    #else
        #define GAME_API __declspec(dllimport)
    #endif
#else
    #define GAME_API
#endif

// Bridge function pointers (passed from main to DLL)
typedef struct {
    void (*InitWindow)(int width, int height, const char *title);
    void (*CloseWindow)(void);
    bool (*WindowShouldClose)(void);
    void (*BeginDrawing)(void);
    void (*DrawText)(const char*text, int posX, int posY, int fontSize, Color color);
    void (*EndDrawing)(void);
    void (*ClearBackground)(Color color);
    int (*GetKeyPressed)(void);  // For 'R' detection
    // Add more raylib funcs as needed (e.g., DrawText, etc.)
} RaylibAPI;

GAME_API void LoadRaylibAPI(const RaylibAPI *api);

GAME_API void GameInit(void);
GAME_API void GameUpdate(void);  // Called every frame
GAME_API int GetReloadFlag(void);  // 1 if 'R' pressed