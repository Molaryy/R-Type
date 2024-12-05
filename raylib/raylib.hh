#pragma once

extern "C"
{
    #include "raylib.h"
}

namespace rtype
{
    class RayLib
    {
        public:
            RayLib() = default;
            ~RayLib() = default;

            void initWindow(int width, int height, const char *title);
            void setTargetFPS(int fps);
            bool windowShouldClose();
            void beginDrawing();
            void drawText(const char *text, int x, int y, int fontSize, Color color);
            void endDrawing();
            void drawCircle(int centerX, int centerY, float radius, Color color);
            void drawRectangle(int x, int y, int width, int height, Color color);
            void drawRectangleRec(Rectangle rec, Color color);
            bool isKeyPressed(KeyboardKey key);
            void beginMode2D(Camera2D camera);
            void clearBackground(Color color);
            float getMouseWheelMove();
            bool checkCollisionPointRec(Vector2 point, Rectangle rec);
            bool isMouseButtonReleased(int button);
            Vector2 getMousePosition();
            Font getFontDefault();
            void drawTextEx(Font font, const char *text, Vector2 position, float fontSize, float spacing, Color color); // NEW
            void closeWindow();
    };
}