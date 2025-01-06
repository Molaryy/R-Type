/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RaylibGraphic.cpp
*/

#include "RaylibGraphic.hpp"

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
        ::EndDrawing();
    }

    void RaylibGraphic::clearBackground(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
        ::ClearBackground(Color{ r, g, b, a });
    }

    void RaylibGraphic::drawRectangle(int x, int y, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
        ::DrawRectangle(x, y, width, height, Color{ r, g, b, a });
    }

    int RaylibGraphic::loadTexture(const std::string &path) {
        Texture2D texture = ::LoadTexture(path.c_str());
        int id = nextTextureID_++;

        textures_[id] = texture;
        return id;
    }

    void RaylibGraphic::unloadTexture(int textureID) {
        auto it = textures_.find(textureID);

        if (it != textures_.end()) {
            ::UnloadTexture(it->second);
            textures_.erase(it);
        }
    }

    void RaylibGraphic::drawTexture(int textureID, int x, int y) {
        auto it = textures_.find(textureID);

        if (it != textures_.end()) {
            ::DrawTexture(it->second, x, y, WHITE);
        }
    }

    void RaylibGraphic::drawText(const std::string &text, int x, int y, int fontSize, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
        ::DrawText(text.c_str(), x, y, fontSize, Color{ r, g, b, a});
    }

    extern "C" {
        #if defined(_WIN32) || defined(_WIN64)
        __declspec(dllexport)
        #else
        __attribute__((visibility("default")))
        #endif
        IRenderer* create_graphic_instance() {
            return new RaylibGraphic();
        }
    }
}
