/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** IRenderer.hpp
*/

#pragma once

#include <string>

namespace Graphic {
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
            virtual void drawTexture(int textureID, int x, int y) = 0;

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

            // TODO complete with what we will need for the game
    };
}
