/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** InputSystem.cpp
*/

#include "Platformer.hpp"
#include "Zipper.hh"

void Platform::inputSystem(Registry &r) {
    auto &positions = r.get_components<Position>();
    auto &velocities = r.get_components<Velocity>();
    auto &entityType = r.get_components<EntityType>();

    auto &renderer = getInstance().getRenderer();
    auto ev = renderer.getEvents();

    bool left = false;
    bool right = false;
    bool jump = false;

    for (auto &k : ev.inputs) {
        if (k == Graphic::Keys::LeftArrow) left = true;
        if (k == Graphic::Keys::RightArrow) right = true;
        if (k == Graphic::Keys::UpArrow) jump = true;
    }

    if (jump && !Platform::getInstance().gameStarted_) {
        Platform::getInstance().gameStarted_ = true;
    }
    if (!Platform::getInstance().gameStarted_) {
        return;
    }
    if (jump) {
        Platform::getInstance().autoJump_ = true;
    }

    for (auto &&[pos, vel, player] : Zipper(positions, velocities, entityType)) {
        if (player.type == PlayerType) {
            if (left && !right) {
                vel.x = -250.f;
            } else if (right && !left) {
                vel.x = 250.f;
            } else {
                vel.x = 0.f;
            }
            if (Platform::getInstance().autoJump_) {
                if (std::fabs(vel.y) < 0.01f) {
                    vel.y = -500.f;
                }
            } else if (jump && std::fabs(vel.y) < 0.01f) {
                vel.y = -500.f;
            }
        }
    }
}
