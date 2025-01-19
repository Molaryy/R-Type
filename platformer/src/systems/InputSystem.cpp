/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** InputSystem.cpp
*/

#include "Platformer.hpp"
#include "Zipper.hh"

void Platform::inputSystem(Registry &r) {
    SparseArray<Position> &positions = r.get_components<Position>();
    SparseArray<Velocity> &velocities = r.get_components<Velocity>();
    SparseArray<EntityType> &entityType = r.get_components<EntityType>();

    Graphic::IRenderer &renderer = getInstance().getRenderer();
    const Graphic::event_t &ev = renderer.getEvents();

    bool left = false;
    bool right = false;
    bool jump = false;

    for (const Graphic::Keys &k : ev.inputs) {
        if (k == Graphic::Keys::LeftArrow)
            left = true;
        if (k == Graphic::Keys::RightArrow)
            right = true;
        if (k == Graphic::Keys::UpArrow)
            jump = true;
    }

    if (jump && !getInstance().gameStarted_)
        getInstance().gameStarted_ = true;
    if (!getInstance().gameStarted_)
        return;
    if (jump)
        getInstance().autoJump_ = true;

    for (auto &&[pos, vel, player] : Zipper(positions, velocities, entityType)) {
        if (player.type == PlayerType) {
            if (left && !right)
                vel.x = -250.f;
            else if (right && !left)
                vel.x = 250.f;
            else
                vel.x = 0.f;
            if (getInstance().autoJump_) {
                if (std::fabs(vel.y) < 0.01f)
                    vel.y = -500.f;
            } else if (jump && std::fabs(vel.y) < 0.01f)
                vel.y = -500.f;
        }
    }
}
