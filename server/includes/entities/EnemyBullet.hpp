/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** EnemyBullet.hpp
*/

#pragma once

#include "Components.hh"
#include "Registry.hh"

#define ENEMY_BULLET_DAMAGE 10
#define ENEMY_BULLET_SPEED 5
#define ENEMY_BULLET_SIZE 15

class EnemyBullet {
public:
    EnemyBullet() = delete;

    static void collision(Registry &r, entity_t me, entity_t other);
    static entity_t create(Registry &r, Position start, Position end, float speed);
};
