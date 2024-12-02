#include <raylib.h>

int main()
{
    InitWindow(800, 600, "Raylib Window");
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello, Raylib works !", 200, 200, 200, ORANGE);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
