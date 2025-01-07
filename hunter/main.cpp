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

Hunter::Hunter(): graphicLoader_("./", "raylib_graphics") {
    try {
        auto createGraphic = graphicLoader_.get_function<Graphic::IRenderer *()>("create_graphic_instance");
        renderer_.reset(createGraphic());
    } catch (const dylib::exception &e){
        throw std::runtime_error("Failed to load shared lib: " + std::string(e.what()));
    }
}

Hunter::~Hunter() {}

void Hunter::run() {
    renderer_->initWindow(800, 600, "Hunter Game POC ECS");
    reg_.register_component<Position_t>();
    reg_.register_component<Velocity_t>();
    reg_.register_component<Collision_t>();
    reg_.register_component<Life_t>();
    reg_.register_component<DuckTag_t>();
    reg_.register_component<Sprite_t>();

    for (int i = 0; i < 3; ++i) {
        entity_t e = reg_.spawn_entity();

        reg_.add_component<Position_t>(e, Position_t{ (float)(50 + i * 100), (float)(100 + i * 50) });
        reg_.add_component<Velocity_t>(e, Velocity_t{ 1.5f, 0.f });
        reg_.add_component<Collision_t>(e, Collision_t{ (int)(50 + i * 100), (int)(100 + i * 50), 64, 64 });
        reg_.add_component<Life_t>(e, Life_t{ 1, 1 });
        reg_.add_component<DuckTag_t>(e, DuckTag_t{});

        int texID = renderer_->loadTexture("assets/duck.png");
        reg_.add_component<Sprite_t>(e, Sprite_t{ texID, 64, 64 });
    }

    while (!renderer_->windowShouldClose()) {
        renderer_->beginDrawing();
        renderer_->clearBackground(0, 120, 120, 255);
        renderer_->endDrawing();
    }
    renderer_->closeWindow();
}

int main() {
    try {
        Hunter game;
        game.run();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 84;
    }
    return 0;
}
