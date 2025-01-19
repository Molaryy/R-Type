/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** BonusTripleShot.hpp
*/

#pragma once

#include "Components.hh"
#include "Registry.hh"

#define BONUS_TRIPLE_SHOT_SIZE 30

class BonusTripleShot {
public:
    BonusTripleShot() = delete;

    static void collision(Registry &r, entity_t me, entity_t other);
    static entity_t create(Registry &r, Position position);
};
