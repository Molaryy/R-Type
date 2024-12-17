/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Components
*/

#pragma once

#include <string>

typedef struct
{
    float x;
    float y;
} Position_t;

typedef struct Velocity
{
    float x;
    float y;
} Velocity_t;

typedef struct Drawable
{
    std::string path;
    int width;
    int height;
} Drawable_t;

typedef struct Controllable
{
    float speed;
} Controllable_t;

typedef struct Life
{
    int current;
    int max;

    void takeDamage(int damage)
    {
        current -= damage;
        if (current < 0)
            current = 0;
    }

    void heal(int heal)
    {
        current += heal;
        if (current > max)
            current = max;
    }

    bool is_alive() const
    {
        return current > 0;
    }
} Life_t;

typedef struct Bonus
{
    enum Type
    {
        HealthRegen,
        DamageBoost,
        SpeedBoost,
    };

    Type type;
    int value;
} Bonus_t;

typedef struct Collision
{
    int x;
    int y;
    int width;
    int height;
    bool is_colliding = false;
} Collision_t;

typedef struct Projectile
{
    int damage;
    float speed;
    bool is_friendly;
    float lifetime;
} Projectile_t;

typedef struct Enemy
{
    int value;
} Enemy_t;

typedef struct SoundEffect
{
    std::string path;
} SoundEffect_t;
