/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RaylibGraphic.hpp
*/

#pragma once

#include <raylib.h>
#include <unordered_map>
#include "IRenderer.hpp"

namespace Graphic {
    class RaylibGraphic final: public IRenderer {
        public:
            RaylibGraphic() = default;
            ~RaylibGraphic() override;

            void initWindow(int width, int height, const std::string &title) override;
            void closeWindow() override;
            bool windowShouldClose() const override;
            void beginDrawing() override;
            void endDrawing() override;
            void clearBackground(unsigned char r, unsigned char g, unsigned char b, unsigned char a) override;
            void drawRectangle(int x, int y, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a) override;
            int loadTexture(const std::string &path) override;
            void unloadTexture(int textureID) override;
            void drawTexture(int textureID, int x, int y) override;
            void drawText(const std::string &text, int x, int y, int fontSize, unsigned char r, unsigned char g, unsigned char b, unsigned char a) override;
            bool isPressed(KeyCode keyCode) const override;
            std::pair<int, int> getMousePosition() override;

        private:
            std::unordered_map<int, Texture2D> textures_;
            int nextTextureID_ = 0;
    };
}
