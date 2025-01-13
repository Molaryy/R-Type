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
        std::function<void(Registry &r)> callback;
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

    struct Entity {
        int x;
        int y;
        std::size_t width;
        std::size_t height;
        void log() const {
            std::cout << "Entity = { x = " << x << ", y = " << y << ", width = " << width << ", height = " << height << " }";
        }
    };

    struct Player {
        std::size_t health;
        std::string name;
        std::size_t score;
        void log() const {
            std::cout << "Player = { health = " << health << ", name = " << name << ", score = " << score << " }";
        }
    };


    struct Bullet {
        int speed;
        int damagePower;
        void log() const {
            std::cout << "Bullet = { x = " << speed << ", y = " << damagePower << " }";
        }
    };

    struct Ennemy {
        std::size_t health;
        void log() const {
            std::cout << "Ennemy = { health = " << health << " }";
        }
    };

    // Game generic components

    /*
    ** Movable allows to entities to be moved by the player
    ** of all the entities in the game, only one can have Movable component, because
    ** only one entity can be moved at a time.
    ** TODO: Add a way to handle multiple entities with Movable component, for example A, W, S, D keys to move the 1 and >, <, ^, v keys to move the other entity
    */
    struct Movable {
        int speed;
        void log() const {
            std::cout << "Movable = { speed = " << speed << " }";
        }
    };

    /*
    ** Drawable allows to entities to be drawn on the screen
    */
    struct Drawable {
        int textureID;
        void log() const {
            std::cout << "Drawable = { textureID = " << textureID << " }";
        }
    };

    /*
    ** Collidable allows to entities to detect if there was a collision with any entity in the game other
    */
    struct Collidable {
        bool isColliding;
        void log() const {
            std::cout << "Collidable = { isColliding = " << isColliding << " }";
        }
    };
};
