/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** EnemyFly.hpp
*/

#pragma once

#include "Registry.hh"

#define FLY_SPEED_X (-1)
#define FLY_SPEED_Y 2
#define FLY_SIZE 30
#define FLY_HEALTH 40

#define FLY_ZIGZAG_SIZE 60

class EnemyFly {
public:
    EnemyFly() = delete;

    struct ArtificialIntelligence {
        std::size_t tick = 0;

        void operator()(Registry &r, entity_t me);
    };
    static void collision(Registry &r, entity_t me, entity_t other);
    static entity_t create(Registry &r);
};
