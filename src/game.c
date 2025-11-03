#include "game.h"
// #include <raylib.h>  // For types, defines, and log levels (e.g., LOG_INFO, Color, RAYWHITE)
#include <stddef.h>

// Note this use pointer as it can't be use normal DrawText as we can refs them.
// There are way to do it.

// Function pointers for raylib functions (set from main to use the shared instance)
static void (*TraceLogPtr)(int logType, const char *text, ...) = NULL;
static void (*DrawTextPtr)(const char *text, int posX, int posY, int fontSize, Color color) = NULL;
static void (*DrawCirclePtr)(int centerX, int centerY, float radius, Color color) = NULL;
static int (*GetScreenWidthPtr)(void) = NULL;
static int (*GetScreenHeightPtr)(void) = NULL;

// Export the setter for raylib functions
void SetRaylibFunctions(
    void (*traceLog)(int logType, const char *text, ...),
    void (*drawText)(const char *text, int posX, int posY, int fontSize, Color color),
    void (*drawCircle)(int centerX, int centerY, float radius, Color color),
    int (*getScreenWidth)(void),
    int (*getScreenHeight)(void)
) {
    TraceLogPtr = traceLog;
    DrawTextPtr = drawText;
    DrawCirclePtr = drawCircle;
    GetScreenWidthPtr = getScreenWidth;
    GetScreenHeightPtr = getScreenHeight;
}

#define TraceLog(...)          if (TraceLogPtr) TraceLogPtr(__VA_ARGS__)
#define DrawText(...)          if (DrawTextPtr) DrawTextPtr(__VA_ARGS__)
#define DrawCircle(...)        if (DrawCirclePtr) DrawCirclePtr(__VA_ARGS__)
#define GetScreenWidth()       (GetScreenWidthPtr ? GetScreenWidthPtr() : 0)
#define GetScreenHeight()      (GetScreenHeightPtr ? GetScreenHeightPtr() : 0)

// Export functions for DLL
void InitGame(void) {
    // Initialization logic (e.g., reset game state)
    if (TraceLogPtr) TraceLogPtr(LOG_INFO, "Game initialized!");
}

void UpdateGame(void) {
    // Update logic (e.g., handle input, update positions)
}

void DrawGame(void) {
    // Draw logic (example: draw some text and a circle)
    if (DrawTextPtr && DrawCirclePtr && GetScreenWidthPtr && GetScreenHeightPtr) {
        // DrawTextPtr("Press R to hot reload!test", 10, 10, 20, DARKGRAY);
        DrawText("Press R to hot reload! test2!", 10, 10, 20, DARKGRAY);
        DrawCirclePtr(GetScreenWidthPtr() / 2, GetScreenHeightPtr() / 2, 50, RED);
    }
}