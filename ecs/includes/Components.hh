/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Components
*/

#pragma once

#include <functional>
#include <iostream>

#include "Registry.hh"

struct Position {
    float x;
    float y;

    void log() const {
        std::cout << "Position = { x = " << x << ", y =" << y << " }";
    }
};

struct Velocity {
    float x;
    float y;

    void log() const {
        std::cout << "Velocity = { x = " << x << ", y = " << y << " }";
    }
};

struct Controllable {
    float speed;

    void log() const {
        std::cout << "Controllable = { speed = " << speed << " }";
    }
};

struct Life {
    int current;
    int max;

    void takeDamage(const int damage) {
        current -= damage;
        if (current < 0)
            current = 0;
    }

    void heal(const int heal) {
        current += heal;
        if (current > max)
            current = max;
    }

    [[nodiscard]] bool is_alive() const {
        return current > 0;
    }

    void log() const {
        std::cout << "Life = { " << current << " / " << max << " }";
    }
};

struct Collision {
    float width;
    float height;

    std::function<void(Registry &r, entity_t me, entity_t other)> collisionTask;

    bool is_colliding = false;


    void log() const {
        std::cout << "Collision = { width = " << width << ", height = " << height << ", is_colliding = " << std::boolalpha << is_colliding << " }";
    }
};

struct Delay {
    int delay = 10;
    int last = 0;

    bool check_activation() {
        if (last >= delay) {
            last = 0;
            return true;
        }
        ++last;
        return false;
    }

    void log() const {
        std::cout << "Delay = { " << last << " / " << delay << " frames }";
    }
};
