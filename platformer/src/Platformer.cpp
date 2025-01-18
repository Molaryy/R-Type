/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Platformer.cpp
*/

#include "Platformer.hpp"
#include "Components.hh"
#include "Zipper.hh"

Platform::Platform(): graphicLoader_("./", "raylib_graphics"), rng_(std::random_device{}()), lastGeneratedY_(350.f) {
    try {
        const auto createGraphic = graphicLoader_.get_function<Graphic::IRenderer *()>("create_instance");
        renderer_.reset(createGraphic());
    } catch (const dylib::exception &e) {
        throw std::runtime_error("Failed to load shared lib: " + std::string(e.what()));
    }
}

Platform::~Platform() {
    if (texturePlayer_ != -1) {
        renderer_->unloadTexture(texturePlayer_);
    }
}

void Platform::initEntities() {
    texturePlayer_ = renderer_->loadTexture("assets/spaceship.gif");

    createPlayer(400.f, 500.f);
    generatePlatformStair(9);
}

void Platform::createPlayer(float x, float y) {
    entity_t e = reg_.spawn_entity();

    reg_.add_component<Position>(e, Position(x, y));
    reg_.add_component<Velocity>(e, Velocity(0.f, 0.f));
    reg_.add_component<Collision>(e, Collision(32, 16));
    reg_.add_component<EntityType>(e, EntityType(PlayerType));
    reg_.add_component<Life>(e, Life{ 1, 1 });
    reg_.add_component<Sprite>(e, Sprite{ texturePlayer_, 32, 16 });
}

void Platform::createPlatform(float x, float y, int w, int h) {
    entity_t e = reg_.spawn_entity();

    reg_.add_component<Position>(e, Position(x, y));
    reg_.add_component<Velocity>(e, Velocity(0.f, 0.f));
    reg_.add_component<Collision>(e, Collision(w, h));
    reg_.add_component<EntityType>(e, EntityType(PlatformType));

    bool doBreakable = ((rng_() % 15) == 0);

    if (doBreakable) {
        reg_.add_component<EntityType>(e, EntityType{ BreakableType, false });
    } else {
        bool doSpring = ((rng_() % 10) == 0);

        if (doSpring) {
            float springW = 20.f;
            float springH = 25.f;
            float sx = x + (w / 2.f) - (springW / 2.f);
            float sy = y + (h - springH);

            createSpring(sx, sy);
        }
    }
}

void Platform::createSpring(float x, float y) {
    entity_t e = reg_.spawn_entity();

    reg_.add_component<Position>(e, Position(x, y));
    reg_.add_component<Velocity>(e, Velocity(0.f, 0.f));
    reg_.add_component<Collision>(e, Collision(20, 10));
    reg_.add_component<EntityType>(e, EntityType(SpringType));
}

void Platform::generatePlatformStair(size_t count) {
    float startY = 450.f;
    float spacingY = 80.f;
    float minX = 50.f;
    float maxX = 800.f - 125.f;
    int w = 100;
    int h = 20;
    float maxDx = 125.f;

    std::uniform_real_distribution<float> distX(-maxDx, maxDx);
    float lastXLine = 400.f;

    for (size_t i = 0; i < count; ++i) {
        float y = startY - i * spacingY;
        float offset = distX(rng_);
        float x = lastXLine + offset;

        createPlatform(x, y, w, h);
        lastXLine = x;
        if ((rng_() % 2) == 0) {
            std::uniform_real_distribution<float> distX(minX, maxX);
            float x2 = distX(rng_);
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
        if (!Platform::getInstance().gameOver_)
            reg_.run_systems();
        else {
            renderingSystem(reg_);
            auto ev = renderer_->getEvents();
            for (auto &key : ev.inputs) {
                if (key == Graphic::Keys::Enter) {
                    restartGame();
                    break;
                } else if (key == Graphic::Keys::Space) {
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
    score_= 0;
    cameraOffsetY_ = 0.f;
    setLastGeneratedY(350.f);
    initEntities();
}

Platform &Platform::createInstance() {
    instance_.reset(new Platform());
    return *instance_;
}

Platform &Platform::getInstance() {
    return *instance_;
}

Graphic::IRenderer &Platform::getRenderer() const {
    return *renderer_;
}

std::unique_ptr<Platform> Platform::instance_ = nullptr;
