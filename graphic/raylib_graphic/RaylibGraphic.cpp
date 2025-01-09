/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RaylibGraphic.cpp
*/

#include "RaylibGraphic.hpp"

#if defined(_WIN32) || defined(_WIN64)
#define LIB_EXPORT __declspec(dllexport)
#else
#define LIB_EXPORT
#endif

namespace Graphic {
    RaylibGraphic::~RaylibGraphic() {
        for (auto &pair : textures_) {
            UnloadTexture(pair.second);
        }
        textures_.clear();
    }

    void RaylibGraphic::initWindow(int width, int height, const std::string &title) {
        ::InitWindow(width, height, title.c_str());
    }

    void RaylibGraphic::closeWindow() {
        ::CloseWindow();
    }

    bool RaylibGraphic::windowShouldClose() const {
        return ::WindowShouldClose();
    }

    void RaylibGraphic::beginDrawing() {
        ::BeginDrawing();
    }

    void RaylibGraphic::endDrawing() {
        EndDrawing();
    }

    void RaylibGraphic::clearBackground(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
        ClearBackground(Color(r, g, b, a));
    }

    void RaylibGraphic::drawRectangle(int x, int y, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
        DrawRectangle(x, y, width, height, Color(r, g, b, a));
    }

    int RaylibGraphic::loadTexture(const std::string &path) {
        Texture2D texture = LoadTexture(path.c_str());
        int id = nextTextureID_++;

        textures_[id] = texture;
        return id;
    }

    void RaylibGraphic::unloadTexture(int textureID) {
        auto it = textures_.find(textureID);

        if (it != textures_.end()) {
            UnloadTexture(it->second);
            textures_.erase(it);
        }
    }

    void RaylibGraphic::drawTexture(int textureID, int x, int y) {
        auto it = textures_.find(textureID);

        if (it != textures_.end()) {
            DrawTexture(it->second, x, y, WHITE);
        }
    }

    void RaylibGraphic::drawText(const std::string &text, int x, int y, int fontSize, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
        DrawText(text.c_str(), x, y, fontSize, Color(r, g, b, a));
    }

    bool RaylibGraphic::isPressed(KeyCode keyCode) const {
        switch (keyCode) {
            case KeyCode::Left:
                return IsKeyDown(KEY_LEFT);
            case KeyCode::Right:
                return IsKeyDown(KEY_RIGHT);
            case KeyCode::Up:
                return IsKeyDown(KEY_UP);
            case KeyCode::Down:
                return IsKeyDown(KEY_DOWN);
            case KeyCode::MouseLeft:
                return IsMouseButtonDown(MOUSE_LEFT_BUTTON);
            case KeyCode::MouseRight:
                return IsMouseButtonDown(MOUSE_RIGHT_BUTTON);
        }
    }

    std::pair<int, int> RaylibGraphic::getMousePosition() {
        Vector2 pos = ::GetMousePosition();

        return { static_cast<int>(pos.x), static_cast<int>(pos.y) };
    }

    extern "C" {
    LIB_EXPORT IRenderer *create_instance() {
        return new RaylibGraphic();
    }
    }
}
