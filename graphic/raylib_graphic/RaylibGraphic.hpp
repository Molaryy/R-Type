/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RaylibGraphic.hpp
*/

#pragma once

#include <raylib.h>
#include <map>
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
            void drawTexture(int textureID, int x, int y, int width, int height, int frame = 0) override;
            void drawText(const std::string &text, int x, int y, int fontSize, unsigned char r, unsigned char g, unsigned char b, unsigned char a) override;
            event_t getEvents() override;
            int loadSound(const std::string &path) override;
            void unloadSound(int soundID) override;
            void playSound(int soundID) override;
            int loadMusic(const std::string &path) override;
            void unloadMusic(int musicID) override;
            void playMusic(int musicID) override;
            void stopMusic(int musicID) override;
            void updateMusic() override;
            float getFrameTime() override;
            void setTargetFps(size_t fps) override;

        private:
            std::unordered_map<int, Texture2D> textures_;
            int nextTextureID_ = 0;

            std::unordered_map<int, Sound> sounds_;
            int nextSoundID_ = 0;

            std::unordered_map<int, Music> musics_;
            int nextMusicID_ = 0;

            inline static const std::map<int, Keys> inputMap_ = {
                {KEY_NULL, Unknown},
                {KEY_A, A}, {KEY_B, B}, {KEY_C, C}, {KEY_D, D},
                {KEY_E, E}, {KEY_F, F}, {KEY_G, G}, {KEY_H, H},
                {KEY_I, I}, {KEY_J, J}, {KEY_K, K}, {KEY_L, L},
                {KEY_M, M}, {KEY_N, N}, {KEY_O, O}, {KEY_P, P},
                {KEY_Q, Q}, {KEY_R, R}, {KEY_S, S}, {KEY_T, T},
                {KEY_U, U}, {KEY_V, V}, {KEY_W, W}, {KEY_X, X},
                {KEY_Y, Y}, {KEY_Z, Z},
                {KEY_ZERO, Num0}, {KEY_ONE, Num1}, {KEY_TWO, Num2},
                {KEY_THREE, Num3}, {KEY_FOUR, Num4}, {KEY_FIVE, Num5},
                {KEY_SIX, Num6}, {KEY_SEVEN, Num7}, {KEY_EIGHT, Num8},
                {KEY_NINE, Num9},
                {KEY_ESCAPE, Escape}, {KEY_LEFT_CONTROL, LControl}, {KEY_LEFT_SHIFT, LShift},
                {KEY_LEFT_ALT, LAlt}, {KEY_LEFT_SUPER, LSystem}, {KEY_RIGHT_CONTROL, RControl},
                {KEY_RIGHT_SHIFT, RShift}, {KEY_RIGHT_ALT, RAlt}, {KEY_RIGHT_SUPER, RSystem},
                {KEY_LEFT_BRACKET, LBracket}, {KEY_RIGHT_BRACKET, RBracket}, {KEY_SEMICOLON, Semicolon},
                {KEY_COMMA, Comma}, {KEY_PERIOD, Period}, {KEY_APOSTROPHE, Quote}, {KEY_SLASH, Slash},
                {KEY_BACKSLASH, Backslash}, {KEY_GRAVE, Tilde}, {KEY_EQUAL, Equal}, {KEY_MINUS, Hyphen},
                {KEY_SPACE, Space}, {KEY_ENTER, Enter}, {KEY_BACKSPACE, Backspace}, {KEY_TAB, Tab},
                {KEY_PAGE_UP, PageUp}, {KEY_PAGE_DOWN, PageDown}, {KEY_END, End}, {KEY_HOME, Home},
                {KEY_INSERT, Insert}, {KEY_DELETE, Delete}, {KEY_KP_ADD, Add}, {KEY_KP_SUBTRACT, Subtract},
                {KEY_KP_MULTIPLY, Multiply}, {KEY_KP_DIVIDE, Divide}, {KEY_LEFT, LeftArrow},
                {KEY_RIGHT, RightArrow}, {KEY_UP, UpArrow}, {KEY_DOWN, DownArrow},
                {KEY_KP_0, Numpad0}, {KEY_KP_1, Numpad1}, {KEY_KP_2, Numpad2},
                {KEY_KP_3, Numpad3}, {KEY_KP_4, Numpad4}, {KEY_KP_5, Numpad5},
                {KEY_KP_6, Numpad6}, {KEY_KP_7, Numpad7}, {KEY_KP_8, Numpad8},
                {KEY_KP_9, Numpad9},
                {KEY_F1, F1}, {KEY_F2, F2}, {KEY_F3, F3},
                {KEY_F4, F4}, {KEY_F5, F5}, {KEY_F6, F6},
                {KEY_F7, F7}, {KEY_F8, F8}, {KEY_F9, F9},
                {KEY_F10, F10}, {KEY_F11, F11}, {KEY_F12, F12},
                {KEY_PAUSE, Pause},
                {MOUSE_BUTTON_LEFT, LeftClick},
                {MOUSE_BUTTON_RIGHT, RightClick}
            };
    };
}
