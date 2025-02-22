/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** hunter.hpp
*/

#pragma once

#include <dylib.hpp>
#include <memory>

#include "IRenderer.hpp"
#include "Registry.hh"

struct DuckTag {
    void log() const;
};

struct Sprite {
    int textureID;
    float width;
    float height;
    float frameTimer;
    float frameDuration;

    void log() const;
};

class Hunter {
public:
    Hunter();
    ~Hunter();

    void run();

    static void duckMovementSystem(Registry &r);
    static void duckShootingSystem(Registry &r);
    static void duckRendererSystem(Registry &r);
    // static void duckAnimationSystem(Registry &r);

    static Hunter &createInstance();
    [[nodiscard]] static Hunter &getInstance();
    [[nodiscard]] Graphic::IRenderer &getRenderer() const;

private:
    dylib graphicLoader_;
    static std::unique_ptr<Hunter> instance_;

    std::unique_ptr<Graphic::IRenderer> renderer_;
    Registry reg_;
};
