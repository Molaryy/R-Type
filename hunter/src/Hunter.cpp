/*
** EPITECH PROJECT, 2025
** B-YEP-400_R-Type
** File description:
** Hunter.cpp
*/

#include "Hunter.hpp"

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

    for (int i = 0; i < 1; ++i) {
        entity_t e = reg_.spawn_entity();

        reg_.add_component<Position>(e, Position(static_cast<float>(0 + i * 100), static_cast<float>(100 + i * 50)));
        reg_.add_component<Velocity>(e, Velocity(0.009f, 0.f));
        reg_.add_component<Collision>(e, Collision(50 + i * 100, 100 + i * 50, 32, 16));
        reg_.add_component<Life>(e, Life(1, 1));
        reg_.add_component<DuckTag>(e, DuckTag());

        reg_.add_component<Sprite>(e, Sprite(renderer_->loadTexture("assets/spaceship.gif"), 32, 16, 0, 0.0f, 0.2f));
    }

    reg_.add_system(duckMovementSystem);
    reg_.add_system(duckShootingSystem);
    // reg_.add_system(duckAnimationSystem);
    reg_.add_system(duckRendererSystem);
    while (!renderer_->windowShouldClose()) {
        reg_.run_systems();
    }
    renderer_->closeWindow();
}

void Hunter::duckMovementSystem(Registry &r) {
    auto &positions = r.get_components<Position>();
    auto &velocities = r.get_components<Velocity>();
    auto &collisions = r.get_components<Collision>();
    auto &ducks = r.get_components<DuckTag>();

    for (auto &&[pos, vel, col, duc] : Zipper(positions, velocities, collisions, ducks)) {
        pos.x += vel.x;
        pos.y += vel.y;
        col.x = pos.x;
        col.y = pos.y;

        if (pos.x > 800)
            pos.x = -64.f;
        if (pos.y > 600)
            pos.y = -16.f;
    }
}

void Hunter::duckShootingSystem(Registry &r) {
    Graphic::IRenderer &renderer = getInstance().getRenderer();
    Graphic::event_t events = renderer.getEvents();
    bool leftClicked = std::find(events.inputs.begin(), events.inputs.end(), Graphic::Keys::LeftClick) != events.inputs.end();

    if (!leftClicked)
        return;

    auto [mouseX, mouseY] = events.mouse_pos;

    auto &ducks = r.get_components<DuckTag>();
    auto &collisions = r.get_components<Collision>();
    auto &lifes = r.get_components<Life>();

    for (auto &&[duck, col, life] : Zipper(ducks, collisions, lifes)) {
        if (mouseX >= col.x && mouseX <= col.x + col.width && mouseY >= col.y && mouseY <= col.y + col.height) {
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
        renderer.drawTexture(sprite.textureID, static_cast<int>(pos.x), static_cast<int>(pos.y), sprite.width, sprite.height, sprite.currentFrame);
    }
    renderer.endDrawing();
}

// void Hunter::duckAnimationSystem(Registry &r) {
//     auto &sprites = r.get_components<Sprite>();

//     for (auto &spriteOpt : sprites) {
//         if (spriteOpt.has_value()) {
//             auto &sprite = spriteOpt.value();

//             sprite.frameTimer += GetFrameTime();
//             if (sprite.frameTimer >= sprite.frameDuration) {
//                 sprite.frameTimer = 0.0f;
//                 sprite.currentFrame = (sprite.currentFrame + 1) % 4;
//             }
//         }
//     }
// }

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
