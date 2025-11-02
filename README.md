# raylib_api_reload_c

# Information:
  Sample of raylib api access for hot reload.

# Guide:

  Build file. Run the application. Then edit game.

  Then use cmd line.
```
cmake --build . --target game
```
  To rebuild the dll library. Press R key to reload library. It to prevent locks in game.dll. You can't overwrite it directly during rebuild—cmake --build would fail with "file in use."

# Notes:
- The drawing code in game.c must use the bridged api.DrawText (not the direct raylib DrawText). Direct calls from the DLL would fail or cause issues because the OpenGL context is owned by the main executable's raylib instance. The bridge passes safe function pointers from main.c, ensuring all rendering happens in the persistent context.

# Credits:
- https://medium.com/@TheElkantor/how-to-add-hot-reload-to-your-raylib-proj-in-c-698caa33eb74
- Raylib