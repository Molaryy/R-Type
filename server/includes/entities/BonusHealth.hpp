/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** BonusHealth.hpp
*/

#pragma once

#include "Components.hh"
#include "Registry.hh"

#define BONUS_HEALTH_SIZE 30
#define BONUS_HEALTH_HEALING 30

class BonusHealth {
public:
    BonusHealth() = delete;

    static void collision(Registry &r, entity_t me, entity_t other);
    static entity_t create(Registry &r, const Position &position);
};
