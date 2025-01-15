/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Player.hpp
*/

#pragma once

#include <bits/stdint-uintn.h>

#include "Registry.hh"

class Player {
public:
    Player() = delete;

    static void collision(Registry &r, entity_t me, entity_t other);
    static entity_t create(Registry &r, uint16_t client_id);
};
