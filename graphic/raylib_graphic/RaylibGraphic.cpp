/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RaylibGraphic.cpp
*/

#include "RaylibGraphic.hpp"
#include <iostream>

#include <ranges>

#if defined(_WIN32) || defined(_WIN64)
#define LIB_EXPORT __declspec(dllexport)
#else
#define LIB_EXPORT
#endif

namespace Graphic {
    RaylibGraphic::~RaylibGraphic() {
        if (IsAudioDeviceReady())
            CloseAudioDevice();
    }

    void RaylibGraphic::initWindow(const int width, const int height, const std::string &title) {
        InitWindow(width, height, title.c_str());
        InitAudioDevice();
    }

    void RaylibGraphic::closeWindow() {
        ::CloseWindow();
    }

    bool RaylibGraphic::windowShouldClose() const {
        return WindowShouldClose();
    }

    void RaylibGraphic::beginDrawing() {
        BeginDrawing();
    }

    void RaylibGraphic::endDrawing() {
        EndDrawing();
    }

    void RaylibGraphic::clearBackground(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a) {
        ClearBackground(Color(r, g, b, a));
    }

    void RaylibGraphic::drawRectangle(const int x, const int y, const int width, const int height, const unsigned char r, const unsigned char g,
                                      const unsigned char b, const unsigned char a) {
        DrawRectangle(x, y, width, height, Color(r, g, b, a));
    }

    int RaylibGraphic::loadTexture(const std::string &path) {
        const Texture2D texture = LoadTexture(path.c_str());
        const int id = nextTextureID_++;

        textures_[id] = texture;
        return id;
    }

    void RaylibGraphic::unloadTexture(int textureID) {
        const auto it = textures_.find(textureID);

        if (it != textures_.end()) {
            UnloadTexture(it->second);
            textures_.erase(it);
        }
    }

    void RaylibGraphic::drawTexture(const int textureID, const int x, const int y, const int width, const int height, const int frame) {
        const auto it = textures_.find(textureID);

        if (it != textures_.end()) {
            const Rectangle srcRec = {0.0f, static_cast<float>(frame * height), static_cast<float>(width), static_cast<float>(height)};
            const Vector2 pos = {static_cast<float>(x), static_cast<float>(y)};

            DrawTextureRec(it->second, srcRec, pos, WHITE);
        }
    }

    void RaylibGraphic::drawText(const std::string &text, const int x, const int y, const int fontSize, const unsigned char r, const unsigned char g,
                                 const unsigned char b, const unsigned char a) {
        DrawText(text.c_str(), x, y, fontSize, Color(r, g, b, a));
    }

    event_t RaylibGraphic::getEvents() {
        event_t events;

        events.mouse_pos = { GetMouseX(), GetMouseY() };
        events.window_size = { GetScreenWidth(), GetScreenHeight() };
        events.inputs.clear();
        if (WindowShouldClose())
            events.inputs.push_back(CloseWindow);

        for (auto const &[raylibKey, customKey] : inputMap_) {
            if (IsKeyDown(raylibKey)) {
                events.inputs.push_back(customKey);
            }
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            events.inputs.push_back(LeftClick);
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            events.inputs.push_back(RightClick);
        }
        return events;
    }

    int RaylibGraphic::loadSound(const std::string &path) {
        const Sound sound = LoadSound(path.c_str());
        const int id = nextSoundID_++;

        sounds_[id] = sound;
        return id;
    }

    void RaylibGraphic::unloadSound(const int soundID) {
        const auto it = sounds_.find(soundID);

        if (it != sounds_.end()) {
            UnloadSound(it->second);
            sounds_.erase(it);
        }
    }

    void RaylibGraphic::playSound(int soundID) {
        auto it = sounds_.find(soundID);

        if (it != sounds_.end()) {
            PlaySound(it->second);
        }
    }

    int RaylibGraphic::loadMusic(const std::string &path) {
        Music music = LoadMusicStream(path.c_str());
        int id = nextMusicID_++;

        musics_[id] = music;
        return id;
    }

    void RaylibGraphic::unloadMusic(int musicID) {
        auto it = musics_.find(musicID);

        if (it != musics_.end()) {
            UnloadMusicStream(it->second);
            musics_.erase(it);
        }
    }

    void RaylibGraphic::playMusic(int musicID) {
        auto it = musics_.find(musicID);

        if (it != musics_.end()) {
            PlayMusicStream(it->second);
        }
    }

    void RaylibGraphic::stopMusic(int musicID) {
        auto it = musics_.find(musicID);

        if (it != musics_.end()) {
            StopMusicStream(it->second);
        }
    }

    void RaylibGraphic::updateMusic() {
        for (auto &pair : musics_) {
            UpdateMusicStream(pair.second);
        }
    }

    extern "C" {
    LIB_EXPORT IRenderer *create_instance() {
        return new RaylibGraphic();
    }
    }
}
