/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Player.hpp
*/

#pragma once

#include <cstdint>

#include "Registry.hh"

#define PLAYER_SPEED 6
#define PLAYER_SIZE_X 33
#define PLAYER_SIZE_Y 17
#define PLAYER_HEALTH 100
#define PLAYER_BULLET_RATE 15

class Player {
public:
    Player() = delete;

    static void collision(Registry &r, entity_t me, entity_t other);
    static entity_t create(Registry &r, uint16_t client_id);
};
