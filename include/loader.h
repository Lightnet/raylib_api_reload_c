#ifndef LOADER_H
#define LOADER_H

void LoadGame(void);
void UnloadGame(void);
void SetGameRaylibFunctions(void);  // New: Set raylib function pointers in the DLL

extern void (*InitGame)(void);
extern void (*UpdateGame)(void);
extern void (*DrawGame)(void);

#endif