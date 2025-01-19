/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RenderingSystem.cpp
*/

#include "Platformer.hpp"
#include "Zipper.hh"

void Platform::renderingSystem(Registry &r) {
    SparseArray<Position> &positions = r.get_components<Position>();
    SparseArray<Collision> &collisions = r.get_components<Collision>();
    SparseArray<Sprite> &sprites = r.get_components<Sprite>();
    SparseArray<EntityType> &entityType = r.get_components<EntityType>();
    Graphic::IRenderer &renderer = getInstance().getRenderer();

    renderer.beginDrawing();
    renderer.clearBackground(100, 200, 255, 255);
    constexpr float groundDrawThreshold = 100.f;

    if (getInstance().cameraOffsetY_ < groundDrawThreshold && !getInstance().gameOver_) {
        constexpr float windowWidth = 800;
        constexpr float groundY = 575;
        const float screenGroundY = groundY + getInstance().cameraOffsetY_;

        renderer.drawRectangle(0, screenGroundY, windowWidth, 50, 101, 67, 33, 255);
        renderer.drawRectangle(0, screenGroundY, windowWidth, 10, 121, 87, 60, 255);
    }
    for (std::size_t e = 0; e < r.max_entities(); ++e) {
        if (!entityType[e].has_value()) continue;
        auto &[type, broken] = entityType[e].value();
        if (!positions[e].has_value()) continue;
        auto &pos = positions[e].value();
        const int screenX = static_cast<int>(pos.x);
        const int screenY = static_cast<int>(pos.y + getInstance().cameraOffsetY_);

        if (type == PlatformType || type == BreakableType) {
            const auto &col = collisions[e].value();
            if (type == BreakableType) {
                renderer.drawRoundedRectangle(screenX, screenY, col.width, col.height, 0.9f, 8, 139, 69, 19, 255);
            } else {
                constexpr int mainR = 0;
                constexpr int mainB = 0;
                constexpr int mainA = 255;
                constexpr int mainG = 255;
                constexpr int borderR = 0;
                constexpr int borderG = 150;
                constexpr int borderB = 0;
                constexpr int borderA = 255;
                constexpr int borderSize = 2;

                renderer.drawRoundedRectangle(
                    screenX - borderSize,
                    screenY - borderSize,
                    col.width + 2 * borderSize,
                    col.height + 2 * borderSize,
                    0.9f, 8, borderR, borderG, borderB, borderA);
                renderer.drawRoundedRectangle(screenX, screenY, col.width, col.height, 0.9f, 8, mainR, mainG, mainB, mainA);
            }
            continue;
        }
        if (type == SpringType) {
            const Collision &col = collisions[e].value();
            renderer.drawRoundedRectangle(screenX, screenY, col.width, col.height, 0.5f, 4, 255, 0, 0, 255);
            continue;
        }
        if (sprites[e].has_value()) {
            const auto &[textureID, width, height] = sprites[e].value();

            renderer.drawTexture(textureID, screenX, screenY, width, height, 0.f, 0.f, width, height);
        } else {
            float w = 32;
            float h = 32;

            if (collisions[e].has_value()) {
                w = collisions[e].value().width;
                h = collisions[e].value().height;
            }
            renderer.drawRectangle(screenX, screenY, w, h, 255, 0, 0, 255);
        }
    }
    if (!getInstance().gameStarted_) {
        renderer.drawText("Press UP to start", 250, 200, 30, 255, 255, 255, 255);
    }
    if (!getInstance().gameOver_) {
        char buffer[64];

        std::snprintf(buffer, 64, "Score: %d", Platform::getInstance().score_);
        renderer.drawText(buffer, 10, 10, 20, 255, 255, 255, 255);
    }
    if (getInstance().gameOver_) {
        char buffer[64];

        renderer.clearBackground(100, 0, 0, 0);
        renderer.drawText("GAME OVER", 250, 100, 50, 255, 255, 255, 255);
        renderer.drawText("Press SPACE key to exit", 225, 200, 30, 255, 255, 255, 255);
        renderer.drawText("Press ENTER key to restart", 185, 240, 30, 255, 255, 255, 255);
        std::snprintf(buffer, sizeof(buffer), "Score: %d", Platform::getInstance().score_);
        renderer.drawText(buffer, 325, 400, 30, 255, 255, 255, 255);
    }
    renderer.endDrawing();
}
