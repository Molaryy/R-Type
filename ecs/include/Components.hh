/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Components
*/

#pragma once

#include <string>
#include <iostream>


typedef struct Position_s
{
    float x;
    float y;
    void log() const {
        std::cout << "Position = { " << x << ", " << y << " }";
    }
} Position_t;

typedef struct Velocity_s
{
    float x;
    float y;
    void log() const {
        std::cout << "Velocity = { " << x << ", " << y << " }";
    }
} Velocity_t;

typedef struct Controllable_s
{
    float speed;
    void log() const {
        std::cout << "Controllable = { " << speed << " }";
    }
} Controllable_t;

typedef struct Life_s
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

    void log() const {
        std::cout << "Life = {" << current << "/" << max << "}";
    }
} Life_t;

typedef struct Collision_s
{
    int x;
    int y;
    int width;
    int height;
    bool is_colliding = false;

    void log() const {
        std::cout << "Controllable = { " << x << ", " << y << " }";
    }
} Collision_t;
