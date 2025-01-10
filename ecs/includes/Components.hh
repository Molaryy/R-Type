/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Components
*/

#pragma once

#include <string>
#include <iostream>


struct Position
{
    float x;
    float y;
    void log() const {
        std::cout << "Position = { " << x << ", " << y << " }";
    }
};

struct Velocity
{
    float x;
    float y;
    void log() const {
        std::cout << "Velocity = { " << x << ", " << y << " }";
    }
};

struct Controllable
{
    float speed;
    void log() const {
        std::cout << "Controllable = { " << speed << " }";
    }
};

struct Life
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

struct Collision
{
    int x;
    int y;
    int width;
    int height;
    bool is_colliding = false;

    void log() const {
        std::cout << "Controllable = { " << x << ", " << y << " }";
    }
};

struct Delay
{
    int delay = 10;
    int last = 0;

    bool can_shoot()
    {
        if (last >= delay)
        {
            last = 0;
            return true;
        }
        ++last;
        return false;
    }

    void log() const {
        std::cout << "Delay = { " << delay << ", " << last << " }";
    }
};
