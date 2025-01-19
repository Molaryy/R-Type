/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** PlayerMovementSystem.cpp
*/

#include "Platformer.hpp"
#include "Zipper.hh"

void Platform::playerMovementSystem(Registry &r) {
    auto &positions = r.get_components<Position>();
    auto &velocities = r.get_components<Velocity>();
    auto &entityType = r.get_components<EntityType>();
    auto &lifes = r.get_components<Life>();

    const float dt = getInstance().dt_;
    constexpr float deathThreshold = 600.f;

    if (!getInstance().gameStarted_) {
        for (auto &&[pos, vel, player] : Zipper(positions, velocities, entityType)) {
            if (player.type == PlayerType) {
                pos.y = 600 - 32;
                vel.y = 0.f;
            }
        }
        return;
    }

    for (auto &&[pos, vel, player] : Zipper(positions, velocities, entityType)) {
        if (player.type == PlayerType) {
            constexpr float gravity = 800.f;
            vel.y += gravity * dt;
            pos.x += vel.x * dt;
            pos.y += vel.y * dt;

            if (pos.x > 800) {
                pos.x -= 800;
            } else if (pos.x + 32 < 0) {
                pos.x += 800;
            }

            const int screenY = static_cast<int>(pos.y + getInstance().cameraOffsetY_);

            if (screenY > static_cast<int>(deathThreshold)) {
                for (auto &&[life] : Zipper(lifes)) {
                    life.takeDamage(life.max);
                    if (!life.is_alive()) {
                        getInstance().gameOver_ = true;
                        getInstance().reg_.clear_entities();
                    }
                }
            }
            if (getInstance().gameStarted_) {
                const float currentHeight = -pos.y;

                if (static_cast<int>(currentHeight) > getInstance().score_)
                    getInstance().score_ = static_cast<int>(currentHeight);
            }
        }
    }
}
