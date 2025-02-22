/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** EnemyTurret.hpp
*/

#pragma once

#include <Components.hh>

#include "Registry.hh"

#define TURRET_BULLET_SPEED 10
#define TURRET_BULLET_SIZE 10
#define TURRET_BULLET_RATE 50

#define TURRET_HEALTH 15
#define TURRET_SIZE 40
#define TURRET_SPEED (-2)

#define TURRET_DROP_BONUS_HEALTH_CHANCE 0.1
#define TURRET_DROP_BONUS_DAMAGE_CHANCE 0.04
#define TURRET_DROP_BONUS_TRIPLE_CHANCE 0.04

class EnemyTurret {
public:
    EnemyTurret() = delete;

    struct ArtificialIntelligence {
        std::size_t tick = 0;

        void operator()(Registry &r, entity_t me);
    };
    static void collision(Registry &r, entity_t me, entity_t other);
    static entity_t create(Registry &r);
    static entity_t createFromPos(Registry &r, const Position &position);
};
