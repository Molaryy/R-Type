/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Platformer.cpp
*/

#include <iostream>
#include "Platformer.hpp"
#include "Components.hh"
#include "Zipper.hh"
#include "IndexedZipper.hh"

Platform::Platform(): graphicLoader_("./", "raylib_graphics"), rng_(std::random_device{}()) {
    try {
        const auto createGraphic = graphicLoader_.get_function<Graphic::IRenderer *()>("create_instance");
        renderer_.reset(createGraphic());

        jumpSoundID_ = renderer_->loadSound("assets/jump.wav");
        // backGroundMusicID_ = renderer_->loadMusic("assets/background_music.mp3");
        // renderer_->playMusic(backGroundMusicID_);
    } catch (const dylib::exception &e) {
        throw std::runtime_error("Failed to load shared lib: " + std::string(e.what()));
    }
}

Platform::~Platform() {
    if (jumpSoundID_ != -1) {
        renderer_->unloadSound(jumpSoundID_);
    }
    // if (backGroundMusicID_ != -1) {
    //     renderer_->stopMusic(backGroundMusicID_);
    //     renderer_->unloadMusic(backGroundMusicID_);
    // }
    if (texturePlayer_ != -1) {
        renderer_->unloadTexture(texturePlayer_);
    }
}

void Platform::initEntities() {
    texturePlayer_ = renderer_->loadTexture("assets/spaceship.gif");

    createPlayer(400.f, 500.f);
    generatePlatformStair(5);
}

void Platform::createPlayer(float x, float y) {
    entity_t e = reg_.spawn_entity();

    reg_.add_component<Position>(e, Position(x, y));
    reg_.add_component<Velocity>(e, Velocity(0.f, 0.f));
    reg_.add_component<Collision>(e, Collision(32, 16));
    reg_.add_component<PlayerTag>(e, PlayerTag());
    reg_.add_component<Sprite>(e, Sprite{ texturePlayer_, 32, 16 });
}

void Platform::createPlatform(float x, float y, int w, int h) {
    entity_t e = reg_.spawn_entity();

    reg_.add_component<Position>(e, Position(x, y));
    reg_.add_component<Velocity>(e, Velocity(0.f, 0.f));
    reg_.add_component<Collision>(e, Collision(w, h));
    reg_.add_component<PlatformTag>(e, PlatformTag());
}

void Platform::generatePlatformStair(size_t count) {
    float startY = 450.f;
    float spacingY = 80.f;
    float minX = 50.f;
    float maxX = 800.f - 100.f;
    int w = 100;
    int h = 20;
    float maxDx = 150.f;

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
    renderer_->setTargetFps(60);
    renderer_->initAudioDevice();

    initEntities();
    reg_.add_system(inputSystem);
    reg_.add_system(playerMovementSystem);
    reg_.add_system(cameraSystem);
    reg_.add_system(platformGenerationSystem);
    reg_.add_system(collisionSystem);
    reg_.add_system(renderingSystem);

    while (!renderer_->windowShouldClose()) {
        renderer_->updateMusic();
        float realDT = renderer_->getFrameTime();

        if (realDT < 1e-9f) realDT = 1.f / 60.f;
        dt_ = realDT;
        reg_.run_systems();
    }
    renderer_->closeWindow();
}

void Platform::inputSystem(Registry &r) {
    auto &positions = r.get_components<Position>();
    auto &velocities = r.get_components<Velocity>();
    auto &players = r.get_components<PlayerTag>();

    auto &renderer = getInstance().getRenderer();
    auto ev = renderer.getEvents();

    bool left = false;
    bool right = false;
    bool jump = false;

    for (auto &k : ev.inputs) {
        if (k == Graphic::Keys::LeftArrow) left = true;
        if (k == Graphic::Keys::RightArrow) right = true;
        if (k == Graphic::Keys::UpArrow) jump = true;
    }

    if (jump) {
        Platform::getInstance().autoJump_ = true;
    }

    for (auto &&[pos, vel, player] : Zipper(positions, velocities, players)) {
        if (left && !right) {
            vel.x = -250.f;
        } else if (right && !left) {
            vel.x = 250.f;
        } else {
            vel.x = 0.f;
        }
        if (Platform::getInstance().autoJump_) {
            if (std::fabs(vel.y) < 0.01f) {
                vel.y = -500.f;
                renderer.playSound(Platform::getInstance().jumpSoundID_);
            }
        } else if (jump && std::fabs(vel.y) < 0.01f) {
            vel.y = -500.f;
            renderer.playSound(Platform::getInstance().jumpSoundID_);
        }
    }
}

void Platform::playerMovementSystem(Registry &r) {
    auto &positions = r.get_components<Position>();
    auto &velocities = r.get_components<Velocity>();
    auto &players = r.get_components<PlayerTag>();

    float dt = getInstance().dt_;
    const float gravity = 800.f;

    for (auto &&[pos, vel, player] : Zipper(positions, velocities, players)) {
        vel.y += gravity * dt;
        pos.x += vel.x * dt;
        pos.y += vel.y * dt;

        if (pos.x < 0) {
            pos.x = 0;
            vel.x = 0;
        }
        if (pos.x > 800 - 32) {
            pos.x = 800 - 32;
            vel.x = 0;
        }
        if (pos.y > 600 - 32) {
            pos.y = 600 - 32;
            vel.y = 0;
        }
    }
}

void Platform::collisionSystem(Registry &r) {
    auto &positions = r.get_components<Position>();
    auto &velocities = r.get_components<Velocity>();
    auto &collisions = r.get_components<Collision>();
    auto &players = r.get_components<PlayerTag>();
    auto &platforms = r.get_components<PlatformTag>();

    float dt = getInstance().dt_;

    for (auto &&[pPos, pVel, pCol, player] : Zipper(positions, velocities, collisions, players)) {
        float left = pPos.x;
        float right = pPos.x + pCol.width;
        float newBottom = pPos.y + pCol.height;
        float oldBottom = newBottom - pVel.y * dt;

        for (auto &&[pos, vel, col, platform] : Zipper(positions, velocities, collisions, platforms)) {
            float platLeft = pos.x;
            float platRight = pos.x + col.width;
            float platTop = pos.y;

            bool overLapX = (right > platLeft && left < platRight);
            bool crossingTop = (oldBottom <= platTop && newBottom >= platTop);

            if (overLapX && crossingTop) {
                pPos.y = platTop - pCol.height;
                pVel.y = 0.f;
            }
        }
    }
}

void Platform::renderingSystem(Registry &r) {
    auto &positions = r.get_components<Position>();
    auto &collisions = r.get_components<Collision>();
    auto &sprites = r.get_components<Sprite>();
    auto &platforms = r.get_components<PlatformTag>();
    auto &renderer = getInstance().getRenderer();

    renderer.beginDrawing();
    renderer.clearBackground(100, 200, 255, 255);
    for (std::size_t e = 0; e < r.max_entities(); ++e) {
        if (!positions[e].has_value()) continue;
        auto &pos = positions[e].value();
        int screenX = static_cast<int>(pos.x);
        int screenY = static_cast<int>(pos.y + Platform::getInstance().cameraOffsetY_);

        if (platforms[e].has_value()) {
            const auto &col = collisions[e].value();

            renderer.drawRoundedRectangle(screenX, screenY, col.width, col.height, 0.9f, 8, 0, 255, 0, 255);
            continue;
        }
        if (sprites[e].has_value()) {
            auto &spr = sprites[e].value();

            renderer.drawTexture(spr.textureID, screenX, screenY, spr.width, spr.height);
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
    renderer.endDrawing();
}

void Platform::cameraSystem(Registry &r) {
    auto &positions = r.get_components<Position>();
    auto &players = r.get_components<PlayerTag>();

    for (auto &&[pos, player] : Zipper(positions, players)) {
        float screenY = pos.y + Platform::getInstance().cameraOffsetY_;
        float safeZoneTop = 80.f;

        if (screenY < safeZoneTop) {
            float delta = safeZoneTop - screenY;

            Platform::getInstance().cameraOffsetY_ += delta;
        }
    }
}

void Platform::platformGenerationSystem(Registry &r) {
    static float lastGeneratedY = 350.f;
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
        lastGeneratedY = newY;
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
