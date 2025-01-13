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

struct Sprite {
    int textureID;
    int width;
    int height;

    void log() const {
        std::cout << "Sprite: textureID=" << textureID;
    }
};

// struct Score {
//     int value;
//     void log() const;
// };

class Platform {
    public:
        Platform();
        ~Platform();

        void run();

        static void inputSystem(Registry &r);
        static void playerMovementSystem(Registry &r);
        static void collisionSystem(Registry &r);
        static void renderingSystem(Registry &r);
        // static void platformGenerationSystem(Registry &r);
        // static void scoreSystem(Registry &r);

        static Platform &createInstance();
        [[nodiscard]] static Platform &getInstance();
        [[nodiscard]] Graphic::IRenderer &getRenderer() const;
        // [[nodiscard]] int getPlayerTextureID() const;

    private:
        dylib graphicLoader_;
        static std::unique_ptr<Platform> instance_;

        std::unique_ptr<Graphic::IRenderer> renderer_;
        Registry reg_;

        // int playerTextureID_;
        // int jumpSoundID_;
        // int backGroundMusicID_;

        int texturePlayer_ = -1;
        int texturePlatform_ = -1;

        float dt_ = 0.f;
        std::mt19937 rng_;

        void initEntities();
        void createPlayer(float x, float y);
        void createPlatform(float x, float y, int w, int h);

};
