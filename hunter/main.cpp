/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main.cpp
*/

#include <cmath>
#include "Registry.hh"
#include "Components.hh"
#include "Systems.hh"
#include "hunter.hpp"
#include "RaylibGraphic.hpp"

inline void duckMovementSystem(Registry &r) {
    auto &positions = r.get_components<Position_t>();
    auto &velocities = r.get_components<Velocity_t>();
    auto &ducks = r.get_components<DuckTag_t>();

    for (std::size_t entity = 0; entity < r.max_entities(); ++entity) {
        if (!ducks[entity].has_value())
            continue;
        if (!positions[entity].has_value())
            continue;
        if (!velocities[entity].has_value())
            continue;

        auto &pos = positions[entity].value();
        auto &vel = velocities[entity].value();

        pos.x += vel.x;
        pos.y += vel.y;
        if (pos.x > 800)
            pos.x = -64.f;
        if (pos.y > 600)
            pos.y = -64.f;
    }
}

inline void duckShootingSystem(Registry &r) {
    bool isClicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    if (!isClicked) {
        return;
    }
    Vector2 mousePos = GetMousePosition();

    auto &ducks = r.get_components<DuckTag_t>();
    auto &collisions = r.get_components<Collision_t>();
    auto &lifes = r.get_components<Life_t>();

    for (std::size_t entity = 0; entity < r.max_entities(); ++entity) {
        if (!ducks[entity].has_value())
            continue;
        if (!collisions[entity].has_value())
            continue;

        auto &col = collisions[entity].value();

        if (mousePos.x >= col.x && mousePos.x <= col.x + col.width && mousePos.y >= col.y && mousePos.y <= col.y + col.height) {
            if (lifes[entity].has_value())
                lifes[entity].value().current = 0;
        }
    }
}

inline void duckRendererSystem(Registry &r, Graphic::IRenderer &gfx) {
    auto &positions = r.get_components<Position_t>();
    auto &lifes = r.get_components<Life_t>();
    auto &sprites = r.get_components<Sprite_t>();

    for (std::size_t entity = 0; entity < r.max_entities(); ++entity) {
        if (!sprites[entity].has_value())
            continue;
        if (!positions[entity].has_value())
            continue;
        if (lifes[entity].has_value() && lifes[entity].value().current <= 0)
            continue;

        auto &pos = positions[entity].value();
        auto &spr = sprites[entity].value();

        gfx.drawTexture(spr.textureID, static_cast<int>(pos.x), static_cast<int>(pos.y));
    }
}

int main() {
    Graphic::RaylibGraphic renderer;

    renderer.initWindow(800, 600, "Hunter Game POC ECS");
    Registry reg;

    reg.register_component<Position_t>();
    reg.register_component<Velocity_t>();
    reg.register_component<Collision_t>();
    reg.register_component<Life_t>();
    reg.register_component<DuckTag_t>();
    reg.register_component<Sprite_t>();

    for (int i = 0; i < 2; i++) {
        entity_t e = reg.spawn_entity();

        reg.add_component<Position_t>(e, Position_t{ (float)(50 + i*100), (float)(100 + i*50) });
        reg.add_component<Velocity_t>(e, Velocity_t{ 1.5f, 0.f });
        reg.add_component<Collision_t>(e, Collision_t{ (int)(50 + i*100), (int)(100 + i*50), 64, 64 });
        reg.add_component<Life_t>(e, Life_t{ 1, 1 });
        reg.add_component<DuckTag_t>(e, DuckTag_t{});

        int texID = renderer.loadTexture("assets/duck.png");
        reg.add_component<Sprite_t>(e, Sprite_t{ texID, 64, 64 });
    }
    while (!renderer.windowShouldClose()) {
        renderer.beginDrawing();
        renderer.clearBackground(0, 120, 120, 255);

        duckMovementSystem(reg);
        duckShootingSystem(reg);
        duckRendererSystem(reg, renderer);
        renderer.endDrawing();
    }

    renderer.closeWindow();
    return 0;
}
