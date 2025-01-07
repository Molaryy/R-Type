/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** hunter.hpp
*/

#pragma once

#include <iostream>
#include <cstdint>

struct DuckTag_t {
    void log() const {
        std::cout << "DuckTag";
    }
};

struct Sprite_t {
    int textureID = -1;
    int width = 64;
    int height = 64;

    void log() const {
        std::cout << "Sprite = { tesxtureID: " << textureID << ", w: " << width << ", h: " << height << "}";
    }
};
