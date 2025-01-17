/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** CameraSystem.cpp
*/

#include "Platformer.hpp"
#include "Zipper.hh"

void Platform::cameraSystem(Registry &r) {
    auto &positions = r.get_components<Position>();
    auto &players = r.get_components<PlayerTag>();

    for (auto &&[pos, player] : Zipper(positions, players)) {
        float screenY = pos.y + Platform::getInstance().cameraOffsetY_;
        float safeZoneTop = 80.f;

        if (screenY < safeZoneTop) {
            float delta = safeZoneTop - screenY;

            Platform::getInstance().cameraOffsetY_ += delta;
        }
    }
}
