#pragma once

#if defined(_WIN32)
    #if defined(BUILD_DLL)
        #define GAME_API __declspec(dllexport)
    #else
        #define GAME_API __declspec(dllimport)
    #endif
#else
    #define GAME_API
#endif

GAME_API void GameInit(void); // Called once (guarded)
GAME_API void GameUpdate(void);  // Called every frame
GAME_API int GetReloadFlag(void);  // 1 if 'R' pressed
GAME_API int GetGameVersion(void);
GAME_API int GetShouldClose(void);  // Close detection
GAME_API void GameCleanup(void);  // Optional uninit