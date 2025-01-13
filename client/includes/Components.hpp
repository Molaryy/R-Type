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

    struct Entity
    {
        int x;
        int y;
        std::size_t width;
        std::size_t height;

        Entity(int x, int y, std::size_t width, std::size_t height)
            : x(x), y(y), width(width), height(height) {}
        void log() const
        {
            std::cout << "Entity = { x = " <<
            x << ", y = " << y <<
            ", width = " << width << ", height = " << height << " }";
        }
    };

    struct Player
    {
        std::size_t health;
        std::string name;
        std::size_t score;

        Player(std::size_t health, std::string name, std::size_t score)
            : health(health), name(name), score(score) {}
        void log() const
        {
            std::cout << "Player = { health = " << health <<
            ", name = " << name << ", score = " << score << " }";
        }
    };

    struct Bullet
    {
        int speed;
        int damagePower;
        void log() const
        {
            std::cout << "Bullet = { x = " << speed << ", y = " << damagePower << " }";
        }
    };

    struct Ennemy
    {
        std::size_t health;
        void log() const
        {
            std::cout << "Ennemy = { health = " << health << " }";
        }
    };

    // Game generic components

    /*
    ** Movable allows to entities to be moved by the player
    ** of all the entities in the game, only one can have Movable component, because
    ** only one entity can be moved at a time.
    ** TODO: Add a way to handle multiple entities with Movable component, for example A, W, S, D keys to move the 1 and >, <, ^, v keys to move the other entity
    ** r: I dont think this is mandatory and compatible with the left time
    */
    struct Movable
    {
        int speed;

        Movable(int speed) : speed(speed) {}
        void log() const
        {
            std::cout << "Movable = { speed = " << speed << " }";
        }
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
