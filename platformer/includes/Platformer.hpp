/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Platformer.hpp
*/

#pragma once

#include <dylib.hpp>
#include <memory>
#include <random>
#include <ctime>
#include <cstdlib>
#include <iostream>

#include "IRenderer.hpp"
#include "Registry.hh"
#include "Components.hh"

struct PlayerTag {
    void log() const {
        std::cout << "PlayerTag";
    }
};

struct PlatformTag {
    void log() const {
        std::cout << "PlatformTag";
    }
};

struct BreakableTag {
    bool broken = false;

    void log() const {
        std::cout << "BreakableTag: broken = " << broken;
    }
};

struct Sprite {
    int textureID;
    int width;
    int height;

    void log() const {
        std::cout << "Sprite: textureID=" << textureID;
    }
};

struct SpringTag {
    void log() const {
        std::cout << "SpringTag";
    }
};

class Platform {
    public:
        Platform();
        ~Platform();

        void run();

        static void inputSystem(Registry &r);
        static void playerMovementSystem(Registry &r);
        static void collisionSystem(Registry &r);
        static void renderingSystem(Registry &r);
        static void platformGenerationSystem(Registry &r);
        static void cameraSystem(Registry &r);

        float getLastGeneratedY() const {
            return lastGeneratedY_;
        }
        void setLastGeneratedY(float y) {
            lastGeneratedY_ = y;
        }

        static Platform &createInstance();
        [[nodiscard]] static Platform &getInstance();
        [[nodiscard]] Graphic::IRenderer &getRenderer() const;

    private:
        dylib graphicLoader_;
        static std::unique_ptr<Platform> instance_;

        std::unique_ptr<Graphic::IRenderer> renderer_;
        Registry reg_;

        int jumpSoundID_ = -1;
        int backGroundMusicID_ = -1;
        int texturePlayer_ = -1;

        float lastGeneratedY_;
        float dt_ = 0.f;
        std::mt19937 rng_;
        float cameraOffsetY_ = 0.f;
        bool autoJump_ = false;
        bool gameStarted_ = false;
        int score_ = 0;
        bool gameOver_ = false;

        void initEntities();
        void createPlayer(float x, float y);
        void createPlatform(float x, float y, int w, int h);
        void createSpring(float x, float y);
        void restartGame();
        void generatePlatformStair(size_t count);
};
