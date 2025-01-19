/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** CameraSystem.cpp
*/

#include "Platformer.hpp"
#include "Zipper.hh"

void Platform::cameraSystem(Registry &r) {
    SparseArray<Position> &positions = r.get_components<Position>();
    SparseArray<EntityType> &entityType = r.get_components<EntityType>();
    float &camera_offset_y = getInstance().cameraOffsetY_;

    for (auto &&[pos, player] : Zipper(positions, entityType)) {
        if (player.type == PlayerType) {
            const float screenY = pos.y + camera_offset_y;
            constexpr float safeZoneTop = 80.f;

            if (screenY < safeZoneTop) {
                const float delta = safeZoneTop - screenY;
                camera_offset_y += delta;
            }
        }
    }
}
