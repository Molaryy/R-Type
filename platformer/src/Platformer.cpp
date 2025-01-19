/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Platformer.cpp
*/

#include "Platformer.hpp"

#include <memory>
#include "Components.hh"

Platform::Platform()
    : graphicLoader_("./", "raylib_graphics"),
      lastGeneratedY_(350.f),
      rng_(std::random_device{}()) {
    try {
        const auto createGraphic = graphicLoader_.get_function<Graphic::IRenderer *()>("create_instance");
        renderer_.reset(createGraphic());
    } catch (const dylib::exception &e) {
        throw std::runtime_error("Failed to load shared lib: " + std::string(e.what()));
    }
}

Platform::~Platform() {
}

void Platform::initEntities() {
    texturePlayer_ = renderer_->loadTexture("assets/spaceship.gif");

    createPlayer(400.f, 500.f);
    generatePlatformStair(9);
}

void Platform::createPlayer(const float x, const float y) {
    const entity_t e = reg_.spawn_entity();

    reg_.add_component<Position>(e, Position(x, y));
    reg_.add_component<Velocity>(e, Velocity(0.f, 0.f));
    reg_.add_component<Collision>(e, Collision(32, 16, [](Registry&, entity_t, entity_t){}));
    reg_.add_component<EntityType>(e, EntityType(PlayerType));
    reg_.add_component<Life>(e, Life{1, 1});
    reg_.add_component<Sprite>(e, Sprite{texturePlayer_, 32, 16});
}

void Platform::createPlatform(const float x, const float y, const float w, const float h) {
    const entity_t e = reg_.spawn_entity();

    reg_.add_component<Position>(e, Position(x, y));
    reg_.add_component<Velocity>(e, Velocity(0.f, 0.f));
    reg_.add_component<Collision>(e, Collision(w, h, [](Registry&, entity_t, entity_t){}));
    reg_.add_component<EntityType>(e, EntityType(PlatformType));

    if (!(rng_() % 15))
        reg_.add_component<EntityType>(e, EntityType{BreakableType, false});
    else if (!(rng_() % 10))
        createSpring(x + w / 2.f - 20.f / 2.f, y + (h - 25.f));
}

void Platform::createSpring(const float x, const float y) {
    const entity_t e = reg_.spawn_entity();

    reg_.add_component<Position>(e, Position(x, y));
    reg_.add_component<Velocity>(e, Velocity(0.f, 0.f));
    reg_.add_component<Collision>(e, Collision(20, 10, [](Registry&, entity_t, entity_t){}));
    reg_.add_component<EntityType>(e, EntityType(SpringType));
}

void Platform::generatePlatformStair(const std::size_t count) {
    constexpr float maxX = 800.f - 125.f;
    constexpr float maxDx = 125.f;

    float lastXLine = 400.f;

    std::uniform_real_distribution distX(-maxDx, maxDx);


    for (std::size_t i = 0; i < count; ++i) {
        constexpr float h = 20;
        constexpr float w = 100;
        constexpr float spacingY = 80.f;
        constexpr float startY = 450.f;
        const float y = startY - static_cast<float>(i) * spacingY;
        const float offset = distX(rng_);
        const float x = lastXLine + offset;

        createPlatform(x, y, w, h);
        lastXLine = x;
        if (rng_() % 2 == 0) {
            constexpr float minX = 50.f;
            distX = std::uniform_real_distribution(minX, maxX);
            const float x2 = distX(rng_);
            createPlatform(x2, y, w, h);
        }
    }
}

void Platform::run() {
    renderer_->initWindow(800, 600, "Platformer");
    renderer_->setTargetFPS(60);

    initEntities();
    reg_.add_system(inputSystem);
    reg_.add_system(playerMovementSystem);
    reg_.add_system(cameraSystem);
    reg_.add_system(platformGenerationSystem);
    reg_.add_system(collisionSystem);
    reg_.add_system(renderingSystem);

    while (!renderer_->windowShouldClose()) {
        float realDT = renderer_->getFrameTime();

        if (realDT < 1e-9f) realDT = 1.f / 60.f;
        dt_ = realDT;
        if (!gameOver_)
            reg_.run_systems();
        else {
            renderingSystem(reg_);
            auto ev = renderer_->getEvents();
            for (const auto &key : ev.inputs) {
                if (key == Graphic::Keys::Enter) {
                    restartGame();
                    break;
                }
                if (key == Graphic::Keys::Space) {
                    renderer_->closeWindow();
                    return;
                }
            }
        }
    }
    renderer_->closeWindow();
}

void Platform::restartGame() {
    reg_.clear_entities();
    gameOver_ = false;
    gameStarted_ = false;
    score_ = 0;
    cameraOffsetY_ = 0.f;
    setLastGeneratedY(350.f);
    initEntities();
}

Platform &Platform::createInstance() {
    instance_ = std::make_unique<Platform>();
    return *instance_;
}

Platform &Platform::getInstance() {
    return *instance_;
}

Graphic::IRenderer &Platform::getRenderer() const {
    return *renderer_;
}

std::unique_ptr<Platform> Platform::instance_ = nullptr;
