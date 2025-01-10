/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** IRenderer.hpp
*/

#pragma once

#include <string>
#include <vector>

namespace Graphic {
    enum class KeyCode : int {
        Left,
        Right,
        Up,
        Down,
        MouseLeft,
        MouseRight,
    };

    enum Keys : int8_t {
        Unknown = -1, // unknown key

        A = 0, B, C, D, E, F, G, H, I,
        J, K, L, M, N, O, P, Q, R, S,
        T, U, V, W, X, Y, Z,

        Num0, Num1, Num2, Num3, Num4,
        Num5, Num6, Num7, Num8, Num9,

        Escape, // Escape key
        LControl, // Left Control key
        LShift, // Left Shift key
        LAlt, // Left Alt key
        LSystem, // Left OS specific key: window (Windows and Linux), apple (MacOS X), ...
        RControl, // Right Control key
        RShift, // Right Shift key
        RAlt, // Right Alt key
        RSystem, // Right OS specific key: window (Windows and Linux), apple (MacOS X), ...
        MenuKey, // Menu key
        LBracket, // [ key
        RBracket, // ] key
        Semicolon, // ; key
        Comma, // , key
        Period, // . key
        Quote, // ' key
        Slash, // / key
        Backslash, // \ key
        Tilde, // ~ key
        Equal, // = key
        Hyphen, // - key (hyphen)
        Space, // Space key
        Enter, // Enter/Return keys
        Backspace, // Backspace key
        Tab, // Tabulation key
        PageUp, // Page up key
        PageDown, // Page down key
        End, // End key
        Home, // Home key
        Insert, // Insert key
        Delete, // Delete key
        Add, // + key
        Subtract, // - key (minus, usually from numpad)
        Multiply, // * key
        Divide, // / key
        LeftArrow, // Left arrow
        RightArrow, // Right arrow
        UpArrow, // Up arrow
        DownArrow, // Down arrow

        Numpad0, // Numpad 0 key
        Numpad1, // Numpad 1 key
        Numpad2, // Numpad 2 key
        Numpad3, // Numpad 3 key
        Numpad4, // Numpad 4 key
        Numpad5, // Numpad 5 key
        Numpad6, // Numpad 6 key
        Numpad7, // Numpad 7 key
        Numpad8, // Numpad 8 key
        Numpad9, // Numpad 9 key

        F1, // F1 key
        F2, // F2 key
        F3, // F3 key
        F4, // F4 key
        F5, // F5 key
        F6, // F6 key
        F7, // F7 key
        F8, // F8 key
        F9, // F9 key
        F10, // F10 key
        F11, // F11 key
        F12, // F12 key
        F13, // F13 key
        F14, // F14 key
        F15, // F15 key
        Pause, // Pause key

        LeftClick, // left mouse button
        RightClick, // right mouse button
        MiddleClick, // middle (wheel) mouse button

        CloseWindow, // Cross button on window

        _NbInputs, // Number of inputs
    };

    struct event_t {
        std::vector<Keys> inputs;
        std::pair<int, int> mouse_pos;
        std::pair<int, int> window_size;
    };

    class IRenderer {
        public:
            virtual ~IRenderer() = default;

            /**
             * @brief 
             * 
             * @param width 
             * @param height 
             * @param title 
             */
            virtual void initWindow(int width, int height, const std::string &title) = 0;

            /**
             * @brief close window and free all ressources
             * 
             */
            virtual void closeWindow() = 0;

            /**
             * @brief 
             * 
             * @return true if the window must close
             * @return false 
             */
            [[nodiscard]] virtual bool windowShouldClose() const = 0;

            /**
             * @brief begin a frame's drawing
             * 
             */
            virtual void beginDrawing() = 0;

            /**
             * @brief end a frame's drawing
             * 
             */
            virtual void endDrawing() = 0;

            /**
             * @brief clear the screen
             * 
             * @param r red
             * @param g green
             * @param b blue
             * @param a alpha
             */
            virtual void clearBackground(unsigned char r, unsigned char g, unsigned char b, unsigned char a) = 0;

            /**
             * @brief draw a rectangle
             * 
             * @param x pos x
             * @param y pos y
             * @param width 
             * @param height 
             * @param r red
             * @param g green
             * @param b blue
             * @param a alpha
             */
            virtual void drawRectangle(int x, int y, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a) = 0;

            /**
             * @brief load texture / sprite
             * 
             * @param path to the file / image
             * @return int 
             */
            virtual int loadTexture(const std::string &path) = 0;

            /**
             * @brief free the texture
             * 
             * @param textureID
             */
            virtual void unloadTexture(int textureID) = 0;

            /**
             * @brief draw texture / sprite
             * 
             * @param textureID 
             * @param x pos x
             * @param y pos y
             */
            virtual void drawTexture(int textureID, int x, int y, int width, int height, int frame = 0) = 0;

            /**
             * @brief draw text
             * 
             * @param text 
             * @param x 
             * @param y 
             * @param fontSize 
             * @param r 
             * @param g 
             * @param b 
             * @param a 
             */
            virtual void drawText(const std::string &text, int x, int y, int fontSize, unsigned char r, unsigned char g, unsigned char b, unsigned char a) = 0;

            /**
             * @brief Get the Events object
             * 
             * @return event_t 
             */
            virtual event_t getEvents() = 0;
    };
}
