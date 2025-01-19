/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** BonusForce.hpp
*/

#pragma once

#include "Components.hh"
#include "Registry.hh"

#define BONUS_FORCE_SIZE 30
#define BONUS_FORCE_DAMAGE_BOOST 30

class BonusForce {
public:
    BonusForce() = delete;

    static void collision(Registry &r, entity_t me, entity_t other);
    static entity_t create(Registry &r, const Position &position);
};
