/*
** EPITECH PROJECT, 2025
** B-YEP-400_R-Type
** File description:
** Hunter.cpp
*/

#include "Hunter.hpp"

#include <raylib.h>
#include "Components.hh"
#include "Zipper.hh"

void DuckTag::log() const {
    std::cout << "DuckTag";
}

void Sprite::log() const {
        std::cout << "Sprite = { tesxtureID: " << textureID << ", w: " << width << ", h: " << height << " }";
}

Hunter::Hunter(): graphicLoader_("./", "raylib_graphics") {
    try {
        const auto createGraphic = graphicLoader_.get_function<Graphic::IRenderer *()>("create_instance");
        renderer_.reset(createGraphic());
    } catch (const dylib::exception &e) {
        throw std::runtime_error("Failed to load shared lib: " + std::string(e.what()));
    }
}

Hunter::~Hunter() {
}

void Hunter::run() {
    renderer_->initWindow(800, 600, "Hunter Game POC ECS");

    for (int i = 0; i < 3; ++i) {
        entity_t e = reg_.spawn_entity();

        reg_.add_component<Position>(e, Position(static_cast<float>(50 + i * 100), static_cast<float>(100 + i * 50)));
        reg_.add_component<Velocity>(e, Velocity(1.5f, 0.f));
        reg_.add_component<Collision>(e, Collision(50 + i * 100, 100 + i * 50, 64, 64));
        reg_.add_component<Life>(e, Life(1, 1));
        reg_.add_component<DuckTag>(e, DuckTag());

        reg_.add_component<Sprite>(e, Sprite(renderer_->loadTexture("assets/duck.png"), 64, 64));
    }

    reg_.add_system(duckMovementSystem);
    reg_.add_system(duckShootingSystem);
    reg_.add_system(duckRendererSystem);
    while (!renderer_->windowShouldClose())
        reg_.run_systems();
    renderer_->closeWindow();
}

void Hunter::duckMovementSystem(Registry &r) {
    auto &positions = r.get_components<Position>();
    auto &velocities = r.get_components<Velocity>();
    auto &ducks = r.get_components<DuckTag>();

    for (auto &&[pos, vel, duc] : Zipper(positions, velocities, ducks)) {
        pos.x += vel.x;
        pos.y += vel.y;
        if (pos.x > 800)
            pos.x = -64.f;
        if (pos.y > 600)
            pos.y = -64.f;
    }
}

void Hunter::duckShootingSystem(Registry &r) {
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        return;
    auto [x, y] = GetMousePosition();

    auto &ducks = r.get_components<DuckTag>();
    auto &collisions = r.get_components<Collision>();
    auto &lifes = r.get_components<Life>();

    for (auto &&[duck, col, life] : Zipper(ducks, collisions, lifes)) {
        if (x >= col.x && x <= col.x + col.width && y >= col.y && y <= col.y + col.height) {
            life.current = 0;
        }
    }
}

void Hunter::duckRendererSystem(Registry &r) {
    auto &positions = r.get_components<Position>();
    auto &lifes = r.get_components<Life>();
    auto &sprites = r.get_components<Sprite>();
    Graphic::IRenderer &renderer = getInstance().getRenderer();

    renderer.beginDrawing();
    renderer.clearBackground(0, 120, 120, 255);

    for (auto &&[pos, life, sprite] : Zipper(positions, lifes, sprites)) {
        if (life.current <= 0)
            continue;

        renderer.drawTexture(sprite.textureID, static_cast<int>(pos.x), static_cast<int>(pos.y));
    }
    renderer.endDrawing();
}

Hunter &Hunter::createInstance() {
    instance_.reset(new Hunter());
    return *instance_;
}

Hunter &Hunter::getInstance() {
    return *instance_;
}

Graphic::IRenderer &Hunter::getRenderer() const {
    return *renderer_;
}

std::unique_ptr<Hunter> Hunter::instance_ = nullptr;
