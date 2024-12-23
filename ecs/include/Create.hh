/*
** EPITECH PROJECT, 2024
** Rtype
** File description:
** Create
*/

#pragma once

#include "Registry.hh"
#include "Components.hh"

class Create
{
public:
    Create();
    ~Create();
    void createPlayer(registry &reg) noexcept
    {
        auto entity = reg.spawn_entity();
        reg.add_component(entity, Position_t{100.0f, 100.0f});
        reg.add_component(entity, Velocity_t{0.0f, 0.0f});
        reg.add_component(entity, Drawable_t{"exemple.png", 32, 32});
        reg.add_component(entity, Controllable_t{200.0f});
        reg.add_component(entity, Life_t{100, 100});
    }

    void createEnemy(registry &reg, float x, float y) noexcept
    {
        auto entity = reg.spawn_entity();
        reg.add_component(entity, Position_t{x, y});
        reg.add_component(entity, Velocity_t{0.0f, 0.0f});
        reg.add_component(entity, Drawable_t{"exemple.png", 32, 32});
        reg.add_component(entity, Life_t{100, 100});
    }
};
