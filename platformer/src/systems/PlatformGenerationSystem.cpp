/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** PlatformGenerationSystem.cpp
*/

#include <IndexedZipper.hh>
#include <queue>

#include "Platformer.hpp"
#include "Zipper.hh"

void Platform::platformGenerationSystem(Registry &r) {
    const float lastGeneratedY = getInstance().getLastGeneratedY();
    auto &positions = r.get_components<Position>();
    auto &entityType = r.get_components<EntityType>();
    float minScreenY = 999999.f;

    for (auto &&[pos, player] : Zipper(positions, entityType)) {
        if (player.type == PlayerType) {
            const float screenY = pos.y + getInstance().cameraOffsetY_;

            if (screenY < minScreenY)
                minScreenY = screenY;
        }
    }

    if (minScreenY < 100.f) {
        const float newY = lastGeneratedY - 100.f;
        std::uniform_real_distribution distX(50.f, 800.f - 100.f);
        const float x = distX(getInstance().rng_);

        getInstance().createPlatform(x, newY, 100, 20);
        if (getInstance().rng_() % 2 == 0) {
            const float x2 = distX(getInstance().rng_);
            getInstance().createPlatform(x2, newY, 100, 20);
        }
        getInstance().setLastGeneratedY(newY);
    }

    constexpr float removeThreshold = 600.f + 50.f;
    std::queue<entity_t> kill_queue;

    for (auto &&[e, type, pos] : IndexedZipper(entityType, positions)) {
        if (type.type == PlatformType) {
            const float screenY = pos.y + getInstance().cameraOffsetY_;
            if (screenY > removeThreshold)
                kill_queue.push(e);
        }
    }

    while (!kill_queue.empty()) {
        r.kill_entity(kill_queue.front());
        kill_queue.pop();
    }
}
