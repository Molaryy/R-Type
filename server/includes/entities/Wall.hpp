/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Wall.hpp
*/

#pragma once

#include "Components.hh"
#include "Registry.hh"

#define WALL_SIZE TILE_SIZE

class Wall {
public:
    Wall() = delete;

    static void collision(Registry &r, entity_t me, entity_t other);
    static entity_t create(Registry &r);
    static entity_t createFromPos(Registry &r, const Position &position);
};
