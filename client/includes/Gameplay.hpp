/*
** EPITECH PROJECT, 2024
** r-type_client
** File description:
** Main.hpp
*/

#pragma once

#include <array>
#include <string>
#include "Scenes.hpp"

struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

// Textures IDs
enum Textures {
    SPACESHIPS,
    SHOOTER_ENEMY,
    FLY_ENEMY,
    TANK_ENEMY,
    ASTEROID_ENEMY,
    PLAYER_BULLET,
    ENEMY_BULLET,
    BACKGROUND,
    EXPLOSION_1,
    BONUS_HEALTH,
    BONUS_FORCE,
    BONUS_TRIPLE_SHOT,
    BACKGROUND_IMAGE,
    NB_TEXTURES
};

inline std::array<std::string, NB_TEXTURES> textures_paths {
    "assets/spaceship.gif",
    "assets/shooter_enemy.gif",
    "assets/fly_enemy.png",
    "assets/tank_enemy.gif",
    "assets/asteroid.gif",
    "assets/player_bullet.gif",
    "assets/enemy_bullet.png",
    "assets/maps/space.png",
    "assets/explosion1.gif",
    "assets/bonus_health.png",
    "assets/bonus_force.png",
    "assets/bonus_triple_shot.png",
    "assets/background.jpg",
};
