/*
** EPITECH PROJECT, 2025
** B-YEP-400_R-Type
** File description:
** Components.hpp
*/

#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

#include "Gameplay.hpp"
#include "Registry.hh"
#include "RTypeProtocol.hpp"

namespace Components {
    struct RenderText {
        std::string text;
        int fontSize;
        bool isDrawable;

        void log() const {
            std::cout << "RenderText = { " "text = " << text << ", " <<
                "fontSize = " << fontSize << "} ";
        }

    };

    struct ColorText {
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

    struct ClickableText {
        std::function<void(Registry &r)> callback;

        void log() const {
            std::cout << "Button = { clicked }" << std::endl;
        }
    };

    struct ColorOverText {
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


    struct Rect {
        Color color;

        void log() const {
            std::cout << "Rect = { "
                "r = " << static_cast<unsigned int>(color.r) << ", " <<
                "g = " << static_cast<unsigned int>(color.g) << ", " <<
                "b = " << static_cast<unsigned int>(color.b) << ", " <<
                "a = " << static_cast<unsigned int>(color.a) << ", " <<
                "}";
        }
    };

    struct InputRect {
        std::string text;

        void log() const {
            std::cout << "InputText = { text = " << text << "}";
        }
    };

    struct ScrollableText {
    };

    struct ServerId {
        std::size_t id;

        void log() const {
            std::cout << "ServerId = { " << id << " }";
        }
    };

    struct ComponentEntityType {
        explicit ComponentEntityType(const Protocol::EntityType &type) {
            this->type = type;
        }

        Protocol::EntityType type;

        void log() const {
            std::cout << "EntityType = { " << typeName_.at(type) << " }";
        }

    private:
        std::unordered_map<Protocol::EntityType, std::string> typeName_{
            {Protocol::PLAYER, "Player"},
            {Protocol::PLAYER_BULLET, "Player Bullet"},
            {Protocol::ENEMY_FLY, "Enemy Fly"},
            {Protocol::ENEMY_TURRET, "Enemy Turret"},
            {Protocol::ENEMY_BULLET, "Enemy Bullet"},
            {Protocol::BOSS_HEART, "Boss Heart"},
            {Protocol::WALL, "Wall"},
        };
    };

    /*
    ** Drawable allows to entities to be drawn on the screen
    */
    struct Drawable {
        int textureID;

        float width;
        float height;

        float text_x;
        float text_y;
        float text_width;
        float text_height;

        bool can_draw;

        void log() const {
            std::cout << "Drawable = { textureID = " << textureID << " }";
        }
    };
};
