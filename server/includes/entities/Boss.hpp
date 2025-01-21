/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Boss.hpp
*/

#pragma once

#include "Components.hh"
#include "Registry.hh"
#include "RTypeProtocol.hpp"

#define BOSS_BULLET_SPEED 20
#define BOSS_BULLET_SIZE 10
#define BOSS_BULLET_RATE 25

#define BOSS_HEALTH 800
#define BOSS_SIZE_X 206
#define BOSS_SIZE_Y HEIGHT
#define BOSS_STOP_POS 650

class Boss {
public:
    Boss() = delete;

    struct ArtificialIntelligence {
        std::size_t tick = 0;

        void operator()(Registry &r, entity_t me);
    };

    static void collision(Registry &r, entity_t me, entity_t other);
    static entity_t create(Registry &r);
    static entity_t createFromPos(Registry &r, const Position &position);
};
