/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Platformer.cpp
*/

#include <raylib.h>
#include <iostream>
#include "Platformer.hpp"
#include "Components.hh"
#include "Zipper.hh"
#include "IndexedZipper.hh"

static float g_dt = 0.0f;

Platform::Platform(): graphicLoader_("./", "raylib_graphics"), rng_(std::random_device{}()), lastGeneratedY_(350.f) {
    try {
        const auto createGraphic = graphicLoader_.get_function<Graphic::IRenderer *()>("create_instance");
        renderer_.reset(createGraphic());

        // jumpSoundID_ = renderer_->loadSound("assets/jump.wav");
        // backGroundMusicID_ = renderer_->loadMusic("assets/background.mp4");

        // renderer_->playMusic(backGroundMusicID_);

    } catch (const dylib::exception &e) {
        throw std::runtime_error("Failed to load shared lib: " + std::string(e.what()));
    }
}

Platform::~Platform() {
    // if (jumpSoundID_ == -1) {
    //     renderer_->unloadSound(jumpSoundID_);
    // }
    // if (backGroundMusicID_ == -1) {
    //     renderer_->stopMusic(backGroundMusicID_);
    //     renderer_->unloadMusic(backGroundMusicID_);
    // }
    if (texturePlayer_ != -1) {
        renderer_->unloadTexture(texturePlayer_);
    }
    if (texturePlatform_ != -1) {
        renderer_->unloadTexture(texturePlatform_);
    }
}

void Platform::initEntities() {
    texturePlayer_ = renderer_->loadTexture("assets/spaceship.gif");
    texturePlatform_ = renderer_->loadTexture("assets/platforms.png");

    createPlayer(400.f, 500.f);
    createPlatform(300.f, 450.f, 200, 20);
    createPlatform(100.f, 350.f, 200, 20);
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
    reg_.add_component<PlatformTag>(e, PlatformTag());
    reg_.add_component<Sprite>(e, Sprite{ texturePlatform_, w, h });
}

void Platform::run() {
    renderer_->initWindow(800, 600, "Platformer");
    renderer_->setTargetFps(60);

    initEntities();
    reg_.add_system(inputSystem);
    reg_.add_system(playerMovementSystem);
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

    for (auto &&[pos, vel, player] : Zipper(positions, velocities, players)) {
        if (left && !right) {
            vel.x = -200.f;
        } else if (right && !left) {
            vel.x = 200.f;
        } else {
            vel.x = 0.f;
        }
        if (jump && std::fabs(vel.y) < 0.01f) {
            vel.y = -500.f;
        }
    }
}

void Platform::playerMovementSystem(Registry &r) {
    auto &positions = r.get_components<Position>();
    auto &velocities = r.get_components<Velocity>();
    auto &players = r.get_components<PlayerTag>();
    auto &lifes = r.get_components<Life>();

    float dt = getInstance().dt_;
    const float gravity = 800.f;
    const float deathThreshold = 600.f;

    if (!Platform::getInstance().gameStarted_) {
        for (auto &&[pos, vel, player] : Zipper(positions, velocities, players)) {
            pos.y = 600 - 32;
            vel.y = 0.f;
        }
        return;
    }

    for (auto &&[pos, vel, player] : Zipper(positions, velocities, players)) {
        auto oldPosX = pos.x;
        vel.y += gravity * dt;
        pos.x += vel.x * dt;
        pos.y += vel.y * dt;

        if (pos.x > 800) {
            pos.x -= 800;
        } else if (pos.x + 32 < 0) {
            pos.x += 800;
        }

        int screenY = static_cast<int>(pos.y + Platform::getInstance().cameraOffsetY_);

        if (screenY > deathThreshold) {
            for (auto &&[life] : Zipper(lifes)) {
                life.takeDamage(life.max);
                if (!life.is_alive()) {
                    Platform::getInstance().gameOver_ = true;
                    Platform::getInstance().reg_.clear_entities();
                }
            }
        }
        if (Platform::getInstance().gameStarted_) {
            float currentHeight = -pos.y;

            if (currentHeight > Platform::getInstance().score_) {
                Platform::getInstance().score_ = (int) currentHeight;
            }
        }
    }
}

void Platform::collisionSystem(Registry &r) {
    auto &positions = r.get_components<Position>();
    auto &velocities = r.get_components<Velocity>();
    auto &collisions = r.get_components<Collision>();
    auto &players = r.get_components<PlayerTag>();
    auto &platforms = r.get_components<PlatformTag>();
    auto &springs = r.get_components<SpringTag>();

    float dt = getInstance().dt_;

    for (auto &&[pPos, pVel, pCol, player] : Zipper(positions, velocities, collisions, players)) {
        if (pVel.y > 0.f) {
            float left = pPos.x;
            float right = pPos.x + pCol.width;
            float top = pPos.y;
            float newBottom = pPos.y + pCol.height;
            float oldBottom = newBottom - pVel.y * dt;

            for (auto &&[pos, vel, col, platform] : Zipper(positions, velocities, collisions, platforms)) {
                float platLeft = pos.x;
                float platRight = pos.x + col.width;
                float platTop = pos.y;
                float platBottom = pos.y + col.height;

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
        int px = static_cast<int>(pos.x);
        int py = static_cast<int>(pos.y);

        if (sprites[e].has_value()) {
            auto &spr = sprites[e].value();

            renderer.drawTexture(spr.textureID, px, py, spr.width, spr.height);
        } else {
            int w = 32;
            int h = 32;

            if (collisions[e].has_value()) {
                w = collisions[e].value().width;
                h = collisions[e].value().height;
            }
            renderer.drawRectangle(px, py, w, h, 255, 0, 0, 255);
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

// void Platform::createPlayer() {
//     entity_t player = reg_.spawn_entity();

//     reg_.add_component<Position>(player, Position(600.0f, 500.0f));
//     reg_.add_component<Velocity>(player, Velocity(0.009f, 0.009f));
//     reg_.add_component<Acceleration>(player, Acceleration(0.0f, 600.0f));
//     reg_.add_component<Collision>(player, Collision(600, 500, 32, 16));
//     reg_.add_component<PlayerTag>(player, PlayerTag());
//     // reg_.add_component<Life>(player, Life(1, 1));
//     reg_.add_component<Sprite>(player, Sprite(playerTextureID_, 32, 16, 0, 0.0f, 0.2f));
// }

// void Platform::createPlatform(float x, float y) {
//     entity_t platform = reg_.spawn_entity();

//     reg_.add_component<Position>(platform, Position(x, y));
//     reg_.add_component<Velocity>(platform, Velocity(0.0f, 0.0f));
//     reg_.add_component<Collision>(platform, Collision(static_cast<int>(x), static_cast<int>(y), 100, 20));
//     reg_.add_component<PlatformTag>(platform, PlatformTag());
// }

// void Platform::run() {
//     renderer_->initWindow(1280, 720, "Platformer");
//     SetTargetFPS(60);
//     playerTextureID_ = renderer_->loadTexture("assets/spaceship.gif");
//     createPlayer();

//     for (size_t i = 0; i < 5; ++i) {
//         float x = static_cast<float>(rng_() % (800 - 100) + 50);
//         float y = static_cast<float>(600 - i * 100);
//         createPlatform(x, y);
//     }
//     reg_.add_system(inputSystem);
//     reg_.add_system(playerMovementSystem);
//     reg_.add_system(collisionSystem);
//     // reg_.add_system(platformGenerationSystem);
//     reg_.add_system(renderingSystem);
//     // reg_.add_system(scoreSystem);

//     while (!renderer_->windowShouldClose()) {
//         // renderer_->updateMusic();
//         g_dt = GetFrameTime();
//         if (g_dt == 0) {
//             g_dt = 0.0333f;
//         }
//         // std::cout << "DeltaTime: " << g_dt << std::endl;
//         reg_.run_systems();
//     }
//     renderer_->closeWindow();
// }

// void Platform::playerMovementSystem(Registry &r) {
//     auto &positions = r.get_components<Position>();
//     auto &velocities = r.get_components<Velocity>();
//     auto &accelerations = r.get_components<Acceleration>();
//     auto &players = r.get_components<PlayerTag>();

//     float dt = g_dt;

//     for (auto &&[pos, vel, acc, player] : Zipper(positions, velocities, accelerations, players)) {
//         vel.y += acc.y * dt;
//         pos.x += vel.x * dt;
//         pos.y += vel.y * dt;

//         float maxVx = 300.0f;

//         if (vel.x > maxVx)
//             vel.x = maxVx;
//         if (vel.x < -maxVx)
//             vel.x = -maxVx;
//         if (pos.x < 0) {
//             pos.x = 0;
//             vel.x = 0;
//         }
//         if (pos.x > 1920 - 32) {
//             pos.x = 1920 - 32;
//             vel.x = 0;
//         }
//         if (pos.y > 1080 - 16) {
//             pos.y = 1080 - 16;
//             vel.y = 0;

//             // auto &lifes = r.get_components<Life>();

//             // for (auto &&[life] : Zipper(lifes)) {
//             //     life.takeDamage(1);
//             //     if (!life.is_alive()) {
//             //         CloseWindow();
//             //     }
//             // }
//         }
//     }
// }

// void Platform::inputSystem(Registry &r) {
//     Graphic::IRenderer &renderer = getInstance().getRenderer();
//     Graphic::event_t events = renderer.getEvents();
//     auto &positions = r.get_components<Position>();
//     auto &velocities = r.get_components<Velocity>();
//     auto &players = r.get_components<PlayerTag>();
//     bool left = false;
//     bool right = false;
//     bool jump = false;

//     for (const auto &input : events.inputs) {
//         if (input == Graphic::Keys::LeftArrow) left = true;
//         if (input == Graphic::Keys::RightArrow) right = true;
//         if (input == Graphic::Keys::UpArrow) jump = true;
//     }
//     for (auto &&[pos, vel, player] : Zipper(positions, velocities, players)) {
//         if (left) {
//             vel.x = -200.0f;
//         }
//         if (right) {
//             vel.x = 200.0f;
//         }
//         if (!right && !left) {
//             vel.x = 0.0f;
//         }
//         if (jump && (std::fabs(vel.y) < 0.01f || pos.y >= 600 - 32 - 1)) {
//             vel.y = -250.0f;
//             std::cout << "Moved up" << std::endl;
//             // renderer.playSound(getInstance().jumpSoundID_);
//         }
//     }
// }

// void Platform::collisionSystem(Registry &r) {
//     auto &positions = r.get_components<Position>();
//     auto &velocities = r.get_components<Velocity>();
//     auto &collisions = r.get_components<Collision>();
//     auto &players = r.get_components<PlayerTag>();
//     auto &platforms = r.get_components<PlatformTag>();

//     for (auto &&[playerPos, playerVel, playerColl, player] : Zipper(positions, velocities, collisions, players)) {
//         for (auto &&[platformPos, platformColl, platform] : Zipper(positions, collisions, platforms)) {
//             bool collision = (playerPos.x < platformPos.x + platformColl.width &&
//                                 playerPos.x + playerColl.width > platformPos.x &&
//                                 playerPos.y + playerColl.height <= platformPos.y &&
//                                 playerPos.y + playerColl.height + playerVel.y * g_dt >= platformPos.y);

//             if (collision && playerVel.y > 0) {
//                 playerPos.y = platformPos.y - playerColl.height;
//                 playerVel.y = 0.0f;
//                 // renderer.playSound(getInstance().jumpSoundID_);
//             }
//         }
//     }
// }

// void Platform::platformGenerationSystem(Registry &r) {
//     static float lastPlatformY = 500.0f;
//     static int platformCount = 0;
//     auto &positions = r.get_components<Position>();
//     auto &players = r.get_components<PlayerTag>();
//     float highestY = 600.0f;

//     for (auto &&[pos, player] : Zipper(positions, players)) {
//         if (pos.y < highestY) {
//             highestY = pos.y;
//         }
//     }

//     if (highestY < lastPlatformY - 100.0f) {
//         float newY = lastPlatformY - 100.0f;
//         std::uniform_real_distribution<float> distX(50.0f, 750.0f);
//         float newX = distX(getInstance().rng_);

//         getInstance().createPlatform(newX, newY);
//         lastPlatformY = newY;
//         platformCount++;
//     }

//     auto &platforms = r.get_components<PlatformTag>();
//     auto &platformPositions = r.get_components<Position>();
//     auto &platformCollisions = r.get_components<Collision>();

//     for (auto &&[id, platformPos, platformColl, platform] : IndexedZipper(platformPositions, platformCollisions, platforms)) {
//         if (platformPos.y > 600.0f) {
//             r.remove_entity(id);
//         }
//     }
// }

// void Platform::renderingSystem(Registry &r) {
//     auto &positions = r.get_components<Position>();
//     auto &sprites = r.get_components<Sprite>();
//     auto &players = r.get_components<PlayerTag>();
//     auto &platforms = r.get_components<PlatformTag>();
//     Graphic::IRenderer &renderer = getInstance().getRenderer();

//     renderer.beginDrawing();
//     renderer.clearBackground(135, 206, 235, 255);

//     for (auto &&[pos, platform] : Zipper(positions, platforms)) {
//         renderer.drawRectangle(static_cast<int>(pos.x), static_cast<int>(pos.y), 100, 20, 139, 69, 19, 255);
//     }

//     for (auto &&[pos, sprite, player] : Zipper(positions, sprites, players)) {
//         renderer.drawTexture(sprite.textureID, static_cast<int>(pos.x), static_cast<int>(pos.y), sprite.width, sprite.height, sprite.currentFrame);
//     }
//     renderer.endDrawing();
// }

// int Platform::getPlayerTextureID() const {
//     return playerTextureID_;
// }

// Platform &Platform::createInstance() {
//     instance_.reset(new Platform());
//     return *instance_;
// }

// Platform &Platform::getInstance() {
//     return *instance_;
// }

// Graphic::IRenderer &Platform::getRenderer() const {
//     return *renderer_;
// }

// std::unique_ptr<Platform> Platform::instance_ = nullptr;
