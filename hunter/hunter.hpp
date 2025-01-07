/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** hunter.hpp
*/

#pragma once

#include <iostream>
#include <cstdint>
#include <dylib.hpp>
#include "IRenderer.hpp"

struct DuckTag_t {
    void log() const {
        std::cout << "DuckTag";
    }
};

struct Sprite_t {
    int textureID;
    int width;
    int height;

    void log() const {
        std::cout << "Sprite = { tesxtureID: " << textureID << ", w: " << width << ", h: " << height << "}";
    }
};

class Hunter {
    public:
        Hunter();
        ~Hunter();

        void run();

    private:
        dylib graphicLoader_;
        std::unique_ptr<Graphic::IRenderer> renderer_;
        Registry reg_;
};
