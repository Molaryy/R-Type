/*
** EPITECH PROJECT, 2025
** B-YEP-400_R-Type
** File description:
** Components.hpp
*/

#pragma once

#include <string>
#include <iostream>
#include "Main.hpp"

namespace Components {
    struct RenderText
    {
        std::string text;
        int x;
        int y;
        int fontSize;

        void log() const {
            std::cout << "RenderText = { " 
                "text = " << text << ", " << 
                "x = " << x << ", " << 
                "y = " << y << ", " << 
               "fontSize = " << fontSize << "} ";
        }
    };

    struct ColorText
    {
       Color color;

        void log() const {
            std::cout << "ColorText = { " 
                "r = " << (unsigned int)color.r << ", " << 
                "g = " << (unsigned int)color.g << ", " << 
                "b = " << (unsigned int)color.b << ", " << 
                "a = " << (unsigned int)color.a << ", " << 
                "}";
        }
    };


    struct ClickableText
    {
        void (*callback)();
        void log() const {
            std::cout << "Button = { clicked }" << std::endl;
        }
    };

    struct ColorOverText
    {
        Color newColor;
        Color defaultColor;
        bool isOver;
        void log() const {
            std::cout << "ColorOverText = { " 
                "r = " << (unsigned int)newColor.r << ", " << 
                "g = " << (unsigned int)newColor.g << ", " << 
                "b = " << (unsigned int)newColor.b << ", " << 
                "a = " << (unsigned int)newColor.a << ", " << 
                "}";
        }
    };
};
