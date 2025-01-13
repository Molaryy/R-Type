/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Shoot.hpp
*/

#pragma once

#include <Components.hh>

#include "Registry.hh"

class Shoot {
public:
    Shoot() = delete;

    static void collision(Registry &r, entity_t me, entity_t other);
    static entity_t create(Registry & r, const Position &position);
};
