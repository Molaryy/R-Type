/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** EnemyFly.hpp
*/

#pragma once

#include <cstdint>

#include "Registry.hh"

class EnemyFly {
public:
    EnemyFly() = delete;

    static void collision(Registry &r, entity_t me, entity_t other);
    static entity_t create(Registry &r);
};
