/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** PlayerBullet.hpp
*/

#pragma once

#include "Components.hh"
#include "Registry.hh"

#define PLAYER_BULLET_DAMAGE 10
#define PLAYER_BULLET_SPEED 40
#define PLAYER_BULLET_SIZE 15

class PlayerBullet {
public:
    PlayerBullet() = delete;

    static void collision(Registry &r, entity_t me, entity_t other);
    static entity_t create(Registry &r, Position position);
};
