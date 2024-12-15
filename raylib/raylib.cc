#include "raylib.hh"

namespace rtype
{
    void RayLib::initWindow(int width, int height, const char *title)
    {
        InitWindow(width, height, title);
    }

    void RayLib::setTargetFPS(int fps)
    {
        SetTargetFPS(fps);
    }

    bool RayLib::windowShouldClose()
    {
        return WindowShouldClose();
    }

    void RayLib::beginDrawing()
    {
        BeginDrawing();
    }

    void RayLib::drawText(const char *text, int x, int y, int fontSize, Color color)
    {
        DrawText(text, x, y, fontSize, color);
    }

    void RayLib::endDrawing()
    {
        EndDrawing();
    }

    void RayLib::drawCircle(int centerX, int centerY, float radius, Color color)
    {
        DrawCircle(centerX, centerY, radius, color);
    }

    void RayLib::drawRectangle(int x, int y, int width, int height, Color color)
    {
        DrawRectangle(x, y, width, height, color);
    }

    void RayLib::drawRectangleRec(Rectangle rec, Color color)
    {
        DrawRectangleRec(rec, color);
    }

    bool RayLib::isKeyPressed(KeyboardKey key)
    {
        return IsKeyPressed(key);
    }

    void RayLib::beginMode2D(Camera2D camera)
    {
        BeginMode2D(camera);
    }

    void RayLib::clearBackground(Color color)
    {
        ClearBackground(color);
    }

    float RayLib::getMouseWheelMove()
    {
        return GetMouseWheelMove();
    }

    bool RayLib::checkCollisionPointRec(Vector2 point, Rectangle rec)
    {
        return CheckCollisionPointRec(point, rec);
    }

    bool RayLib::isMouseButtonReleased(int button)
    {
        return IsMouseButtonReleased(button);
    }

    Vector2 RayLib::getMousePosition()
    {
        return GetMousePosition();
    }

    Font RayLib::getFontDefault()
    {
        return GetFontDefault();
    }

    void RayLib::drawTextEx(Font font, const char *text, Vector2 position, float fontSize, float spacing, Color color)
    {
        DrawTextEx(font, text, position, fontSize, spacing, color);
    }

    void RayLib::closeWindow()
    {
        CloseWindow();
    }

    void rtype::RayLib::drawCube(Vector3 position, float width, float height, float length, Color color)
    {
        DrawCube(position, width, height, length, color);
    }

    void rtype::RayLib::drawCubeWires(Vector3 position, float width, float height, float length, Color color)
    {
        DrawCubeWires(position, width, height, length, color);
    }

    void rtype::RayLib::drawGrid(int slices, float spacing)
    {
        DrawGrid(slices, spacing);
    }

    bool rtype::RayLib::isKeyDown(KeyboardKey key)
    {
        return IsKeyDown(key);
    }

    float rtype::RayLib::getFrameTime()
    {
        return GetFrameTime();
    }

    void rtype::RayLib::drawCubeMap(TextureCubemap cubeMap, Vector3 position, float size)
    {
        rlDrawRenderBatchActive();
        rlDisableBackfaceCulling();
        DrawCube(position, size, size, size, WHITE);
        rlEnableBackfaceCulling();
    }

    void rtype::RayLib::unloadTextureCubemap(TextureCubemap cubeMap)
    {
        UnloadTexture(cubeMap);
    }

    TextureCubemap rtype::RayLib::loadTextureCubemap(const char *fileName, int layoutType)
    {
        Image image = LoadImage(fileName);
        TextureCubemap cubemap = LoadTextureCubemap(image, layoutType);
        UnloadImage(image);
        return cubemap;
    }

    Vector3 rtype::RayLib::vector3Zero()
    {
        return (Vector3){0.0f, 0.0f, 0.0f};
    }

    void rtype::RayLib::SetWindowSize(int width, int height)
    {
        SetWindowSize(width, height);
    }
}