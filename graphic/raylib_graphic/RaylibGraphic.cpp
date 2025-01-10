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

    void RaylibGraphic::drawTexture(int textureID, int x, int y, int width, int height, int frame) {
        auto it = textures_.find(textureID);

        if (it != textures_.end()) {
            Rectangle srcRec = { 0.0f, static_cast<float>(frame * height), static_cast<float>(width), static_cast<float>(height) };
            Vector2 pos = { static_cast<float>(x), static_cast<float>(y) };

            DrawTextureRec(it->second, srcRec, pos, WHITE);
        }
    }

    void RaylibGraphic::drawText(const std::string &text, int x, int y, int fontSize, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
        DrawText(text.c_str(), x, y, fontSize, Color(r, g, b, a));
    }

    event_t RaylibGraphic::getEvents() {
        event_t events;

        events.mouse_pos = { GetMouseX(), GetMouseY() };
        events.window_size = { GetScreenWidth(), GetScreenHeight() };
        if (WindowShouldClose())
            events.inputs.push_back(Keys::CloseWindow);

        for (auto const &[raylibKey, customKey] : RaylibGraphic::inputMap_) {
            if (IsKeyDown(raylibKey)) {
                events.inputs.push_back(customKey);
            }
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            events.inputs.push_back(Keys::LeftClick);
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            events.inputs.push_back(Keys::RightClick);
        }
        return events;
    }

    extern "C" {
    LIB_EXPORT IRenderer *create_instance() {
        return new RaylibGraphic();
    }
    }
}
