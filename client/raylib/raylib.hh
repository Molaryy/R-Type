#pragma once

extern "C"
{
    #include <raylib.h>
    #include <rlgl.h>
}

namespace rtype
{
    class RayLib
    {
    public:
        RayLib() = default;
        ~RayLib() = default;

        // Window management
        void initWindow(int width, int height, const char *title);
        void setWindowSize(int width, int height);
        void closeWindow();
        bool windowShouldClose();

        // Drawing
        void beginDrawing();
        void endDrawing();
        void clearBackground(Color color);

        // 2D Drawing
        void drawText(const char *text, int x, int y, int fontSize, Color color);
        void drawTextEx(Font font, const char *text, Vector2 position, float fontSize, float spacing, Color color);
        void drawCircle(int centerX, int centerY, float radius, Color color);
        void drawRectangle(int x, int y, int width, int height, Color color);
        void drawRectangleRec(Rectangle rec, Color color);
        void drawTexture(Texture2D texture, int posX, int posY, Color tint);
        void drawTextureEx(Texture2D texture, Vector2 position, float rotation, float scale, Color tint);
        void drawTextureRec(Texture2D texture, Rectangle source, Vector2 position, Color tint);
        void drawTexturePro(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint);

        // 3D Drawing
        void drawCube(Vector3 position, float width, float height, float length, Color color);
        void drawCubeWires(Vector3 position, float width, float height, float length, Color color);
        void drawCubeTexture(Texture2D cubemap, Vector3 position, float size, Color tint);
        void drawCubeMap(TextureCubemap cubeMap, Vector3 position, float size);
        void drawGrid(int slices, float spacing);

        // Camera 2D mode
        void beginMode2D(Camera2D camera);

        // Input handling
        bool isKeyPressed(KeyboardKey key);
        bool isKeyDown(KeyboardKey key);
        bool isMouseButtonReleased(int button);
        Vector2 getMousePosition();
        float getMouseWheelMove();

        // Collision detection
        bool checkCollisionPointRec(Vector2 point, Rectangle rec);

        // Font
        Font getFontDefault();

        // Texture
        Texture2D loadTexture(const char *filePath);
        void unloadTexture(Texture2D texture);
        TextureCubemap loadTextureCubemap(const char *fileName, int layoutType);
        void unloadTextureCubemap(TextureCubemap cubeMap);

        // Timing
        void setTargetFPS(int fps);
        float getFrameTime();

        // Vector
        Vector3 vector3Zero();
    };
}
