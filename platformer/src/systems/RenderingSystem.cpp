/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RenderingSystem.cpp
*/

#include "Platformer.hpp"
#include "Zipper.hh"

void Platform::renderingSystem(Registry &r) {
    auto &positions = r.get_components<Position>();
    auto &collisions = r.get_components<Collision>();
    auto &sprites = r.get_components<Sprite>();
    auto &platforms = r.get_components<PlatformTag>();
    auto &springs = r.get_components<SpringTag>();
    auto &breakables  = r.get_components<BreakableTag>();
    auto &renderer = getInstance().getRenderer();

    renderer.beginDrawing();
    renderer.clearBackground(100, 200, 255, 255);
    const int groundY = 575;
    const int windowWidth = 800;
    const float groundDrawThreshold = 100.f;

    if (Platform::getInstance().cameraOffsetY_ < groundDrawThreshold && !Platform::getInstance().gameOver_) {
        int screenGroundY = static_cast<int>(groundY + Platform::getInstance().cameraOffsetY_);

        renderer.drawRectangle(0, screenGroundY, windowWidth, 50, 101, 67, 33, 255);
        renderer.drawRectangle(0, screenGroundY, windowWidth, 10, 121, 87, 60, 255);
    }
    for (std::size_t e = 0; e < r.max_entities(); ++e) {
        if (!positions[e].has_value()) continue;
        auto &pos = positions[e].value();
        int screenX = static_cast<int>(pos.x);
        int screenY = static_cast<int>(pos.y + Platform::getInstance().cameraOffsetY_);

        if (platforms[e].has_value()) {
            const auto &col = collisions[e].value();
            if (breakables[e].has_value()) {
                renderer.drawRoundedRectangle(screenX, screenY, col.width, col.height, 0.9f, 8, 139, 69, 19, 255);
            } else {
                int mainR = 0, mainG = 255, mainB = 0, mainA = 255;
                int borderR = 0, borderG = 150, borderB = 0, borderA = 255;
                int borderSize = 2;

                renderer.drawRoundedRectangle(
                    screenX - borderSize,
                    screenY - borderSize,
                    col.width + 2 * borderSize,
                    col.height + 2 * borderSize,
                    0.9f, 8, borderR, borderG, borderB, borderA
                );
                renderer.drawRoundedRectangle(screenX, screenY, col.width, col.height, 0.9f, 8, mainR, mainG, mainB, mainA);
            }
            continue;
        }
        if (springs[e].has_value()) {
            auto &col = collisions[e].value();
            renderer.drawRoundedRectangle(screenX, screenY, col.width, col.height, 0.5f, 4, 255, 0, 0, 255);
            continue;
        }
        if (sprites[e].has_value()) {
            auto &spr = sprites[e].value();

            renderer.drawTexture(spr.textureID, screenX, screenY, spr.width, spr.height, 0.f, 0.f, spr.width, spr.height);
        } else {
            int w = 32;
            int h = 32;

            if (collisions[e].has_value()) {
                w = collisions[e].value().width;
                h = collisions[e].value().height;
            }
            renderer.drawRectangle(screenX, screenY, w, h, 255, 0, 0, 255);
        }
    }
    if (!Platform::getInstance().gameStarted_) {
        renderer.drawText("Press UP to start", 250, 200, 30, 255, 255, 255, 255);
    }
    if (!Platform::getInstance().gameOver_) {
        char buffer[64];

        std::snprintf(buffer, 64, "Score: %d", Platform::getInstance().score_);
        renderer.drawText(buffer, 10, 10, 20, 255, 255, 255, 255);
    }
    if (Platform::getInstance().gameOver_) {
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
