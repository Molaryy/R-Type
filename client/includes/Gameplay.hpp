/*
** EPITECH PROJECT, 2024
** r-type_client
** File description:
** Main.hpp
*/

#pragma once

#include <array>
#include <string>

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
    ASTEROID_ENEMY,
    PLAYER_BULLET,
    ENNEMY_BULLET,
    BACKGROUND,
    EXPLOSION_1,
    NB_TEXTURES
};

inline std::array<std::string, NB_TEXTURES> textures_paths {
    "assets/spaceship.gif",
    "assets/shooter_enemy.gif",
    "assets/fly_enemy.gif",
    "assets/asteroid.gif",
    "assets/player_bullet.gif",
    "assets/ennemy_bullet.png",
    "assets/maps/space.png",
    "assets/explosion1.gif",
};
