/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** EnemyTank.hpp
*/

#pragma once

#include "Registry.hh"

#define TANK_SIZE 100
#define TANK_SPEED (-1)
#define TANK_HEALTH 500

class EnemyTank {
public:
    EnemyTank() = delete;

    struct ArtificialIntelligence {
        std::size_t tick = 0;

        void operator()(Registry &r, entity_t me);
    };
    static void collision(Registry &r, entity_t me, entity_t other);
    static entity_t create(Registry &r);
};
