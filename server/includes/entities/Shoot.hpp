/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Shoot.hpp
*/

#pragma once

#include <RTypeProtocol.hpp>

#include "Components.hh"
#include "Registry.hh"

#define BULLET_DAMAGE 10
#define BULLET_SPEED 40
#define BULLET_SIZE 15

class Shoot {
public:
    Shoot() = delete;

    static void collision(Registry &r, entity_t me, entity_t other);
    static entity_t create(Registry &r, Position position, Protocol::EntityType type);
};
