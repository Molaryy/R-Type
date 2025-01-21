/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** BossHeart.hpp
*/

#pragma once

#include "Components.hh"
#include "Registry.hh"

#define BOSS_HEART_BULLET_SPEED 20
#define BOSS_HEART_BULLET_SIZE 10
#define BOSS_HEART_BULLET_RATE 60

#define BOSS_HEART_HEALTH 200
#define BOSS_HEART_SIZE 100

#define BOSS_HEART_STOP_POS 600

#define BOSS_HEART_DROP_BONUS_HEALTH_CHANCE 0.5

class BossHeart {
public:
    BossHeart() = delete;

    struct ArtificialIntelligence {
        std::size_t tick = 0;

        void operator()(Registry &r, entity_t me);
    };
    static void collision(Registry &r, entity_t me, entity_t other);
    static entity_t create(Registry &r);
    static entity_t createFromPos(Registry &r, const Position &position);
};
