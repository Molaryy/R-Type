/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** PlatformGenerationSystem.cpp
*/

#include "Platformer.hpp"
#include "Zipper.hh"

void Platform::platformGenerationSystem(Registry &r) {
    float lastGeneratedY = Platform().getInstance().getLastGeneratedY();
    auto &positions = r.get_components<Position>();
    auto &players = r.get_components<PlayerTag>();
    float minScreenY = 999999.f;

    for (auto &&[pos, player] : Zipper(positions, players)) {
        float screenY = pos.y + Platform::getInstance().cameraOffsetY_;

        if (screenY < minScreenY) {
            minScreenY = screenY;
        }
    }

    if (minScreenY < 100.f) {
        float newY = lastGeneratedY - 100.f;
        std::uniform_real_distribution<float> distX(50.f, 800.f - 100.f);
        float x = distX(Platform::getInstance().rng_);

        Platform::getInstance().createPlatform(x, newY, 100, 20);
        if (Platform::getInstance().rng_() % 2 == 0) {
            float x2 = distX(Platform::getInstance().rng_);
            Platform::getInstance().createPlatform(x2, newY, 100, 20);
        }
        Platform().getInstance().setLastGeneratedY(newY);
    }

    auto &platforms = r.get_components<PlatformTag>();
    const float removeThreshold = 600.f + 50.f;

    for (std::size_t e = 0; e < r.max_entities(); ++e) {
        if (!platforms[e].has_value()) continue;
        if (!positions[e].has_value()) continue;
        auto &pos = positions[e].value();
        float screenY = pos.y + Platform::getInstance().cameraOffsetY_;

        if (screenY > removeThreshold) {
            r.kill_entity(static_cast<entity_t>(e));
        }
    }
}
