/*
** EPITECH PROJECT, 2025
** B-YEP-400_R-Type
** File description:
** Components.hpp
*/

#pragma once

#include <string>
#include <iostream>

namespace Components {
    struct RenderText
    {
        std::string text;
        int x;
        int y;
        int fontSize;
//        unsigned char r;
//        unsigned char g;
//        unsigned char b;
//        unsigned char a;
//
        void log() const {
            std::cout << "RenderText = { " 
                "text = " << text << ", " << 
                "x = " << x << ", " << 
                "y = " << y << ", " << 
               "fontSize = " << fontSize << "} ";
 //               "r = " << r << ", " << 
 //               "g = " << g << ", " << 
 //               "b = " << b << ", " << 
 //               "a = " << a << ", " << 
        }
    };

    struct ColorText
    {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;

        void log() const {
            std::cout << "ColorText = { " 
                "r = " << (unsigned int)r << ", " << 
                "g = " << (unsigned int)g << ", " << 
                "b = " << (unsigned int)b << ", " << 
                "a = " << (unsigned int)a << ", " << 
                "}";
        }
    };
};
