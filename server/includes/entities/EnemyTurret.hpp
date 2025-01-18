/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** EnemyTurret.hpp
*/

#pragma once

#include "Registry.hh"

#define TURRET_BULLET_SPEED 1
#define TURRET_BULLET_SIZE 15
#define TURRET_BULLET_RATE 40

#define TURRET_HEALTH 4
#define TURRET_SIZE 40

class EnemyTurret {
public:
    EnemyTurret() = delete;

    struct ArtificialIntelligence {
        std::size_t tick = 0;

        void operator()(Registry &r, entity_t me);
    };
    static void collision(Registry &r, entity_t me, entity_t other);
    static entity_t create(Registry &r);
};
