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

    static std::map<int, Keys> inputMap_ = {
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

    event_t RaylibGraphic::getEvents() {
        event_t events;

        events.mouse_pos = { GetMouseX(), GetMouseY() };
        events.window_size = { GetScreenWidth(), GetScreenHeight() };
        if (WindowShouldClose())
            events.inputs.push_back(CloseWindow);

        for (auto const &[raylibKey, customKey] : inputMap_) {
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
