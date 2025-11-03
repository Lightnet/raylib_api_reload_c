#ifndef GAME_H
#define GAME_H

#include <raylib.h>  // For raylib function addresses

#ifdef __cplusplus
extern "C" {
#endif


#ifdef _WIN32
#  define EXPORT __declspec(dllexport)
#else
#  define EXPORT __attribute__((visibility("default")))
#endif

EXPORT void InitGame(void);
EXPORT void UpdateGame(void);
EXPORT void DrawGame(void);
EXPORT void SetRaylibFunctions(  // New: To receive raylib function pointers from main
    void (*traceLog)(int logType, const char *text, ...),
    void (*drawText)(const char *text, int posX, int posY, int fontSize, Color color),
    void (*drawCircle)(int centerX, int centerY, float radius, Color color),
    int (*getScreenWidth)(void),
    int (*getScreenHeight)(void)
);

#ifdef __cplusplus
}
#endif


#endif