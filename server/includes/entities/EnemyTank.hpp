/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** EnemyTank.hpp
*/

#pragma once

#include <Components.hh>

#include "Registry.hh"

#define TANK_SIZE 100
#define TANK_SPEED (-1)
#define TANK_HEALTH 500


#define TANK_DROP_BONUS_HEALTH_CHANCE 0.3
#define TANK_DROP_BONUS_DAMAGE_CHANCE 0.2
#define TANK_DROP_BONUS_TRIPLE_CHANCE 0.2

class EnemyTank {
public:
    EnemyTank() = delete;

    struct ArtificialIntelligence {
        std::size_t tick = 0;

        void operator()(Registry &r, entity_t me);
    };
    static void collision(Registry &r, entity_t me, entity_t other);
    static entity_t create(Registry &r);
    static entity_t createFromPos(Registry &r, const Position &position);
};
