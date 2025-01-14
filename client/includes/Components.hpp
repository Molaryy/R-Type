/*
** EPITECH PROJECT, 2025
** B-YEP-400_R-Type
** File description:
** Components.hpp
*/

#pragma once

#include <iostream>
#include <string>

#include "Gameplay.hpp"
#include "RTypeProtocol.hpp"

namespace Components
{
    struct RenderText
    {
        std::string text;
        int x;
        int y;
        int fontSize;

        void log() const
        {
            std::cout << "RenderText = { " "text = " << text << ", " <<
            "x = " << x << ", " << "y = " << y << ", " <<
            "fontSize = " << fontSize << "} ";
        }
    };

    struct ColorText
    {
        Color color;

        void log() const {
            std::cout << "ColorText = { "
                "r = " << static_cast<unsigned int>(color.r) << ", " <<
                "g = " << static_cast<unsigned int>(color.g) << ", " <<
                "b = " << static_cast<unsigned int>(color.b) << ", " <<
                "a = " << static_cast<unsigned int>(color.a) << ", " <<
                "}";
        }
    };

    struct ClickableText
    {
        std::function<void(Registry &r)> callback;
        void log() const
        {
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
                "r = " << static_cast<unsigned int>(newColor.r) << ", " <<
                "g = " << static_cast<unsigned int>(newColor.g) << ", " <<
                "b = " << static_cast<unsigned int>(newColor.b) << ", " <<
                "a = " << static_cast<unsigned int>(newColor.a) << ", " <<
                "}";
        }
    };

    struct ScrollableText
    {
    };

    struct ServerId
    {
        std::size_t id;
    };

    struct EntityType
    {
        Protocol::EntityType type;
    };

    /*
    ** Drawable allows to entities to be drawn on the screen
    */
    struct Drawable
    {
        int textureID;

        Drawable(int textureID) : textureID(textureID) {}
        void log() const
        {
            std::cout << "Drawable = { textureID = " << textureID << " }";
        }
    };
};
