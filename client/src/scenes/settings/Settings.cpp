/*
** EPITECH PROJECT, 2025
** R_Type
** File description:
** Settings scene
*/

#include "Client.hpp"
#include "Components.hpp"
#include "Components.hh"
#include "Scenes.hpp"

void settingsCallback(Registry &r)
{
    r.clear_entities();
    const entity_t e = r.spawn_entity();

    r.add_component(e, Components::RenderText("Settings", 40));
    r.add_component(e, Position(50, 50));
    r.add_component(e, Components::ColorText({255, 255, 255, 255}));

    const std::vector<std::string> settingsOptions = {
        "Change FPS",
        "Toggle Music",
        "Toggle Sound Effects",
        "Change Resolution",
        "Toggle Colorblind Mode",
    };

    // TODO move to settings class
    static int currentFPSIndex = 1;
    static bool musicEnabled = true;
    static bool soundEffectsEnabled = true;
    static bool colorBlindMode = false;
    static int currentResolutionIndex = 0;
    auto &renderer = Client::getInstance().getRenderer();

    std::vector<std::function<void()>> actions = {
        [&renderer] {
            constexpr int maxFPSOptions[] = {30, 60, 120};
            currentFPSIndex = (currentFPSIndex + 1) % std::size(maxFPSOptions);
            renderer.setTargetFPS(maxFPSOptions[currentFPSIndex]);
            std::cout << "FPS set to: " << maxFPSOptions[currentFPSIndex] << std::endl;
        },
        [&renderer] {
            musicEnabled = !musicEnabled;
            std::cout << "Music " << (musicEnabled ? "enabled" : "disabled") << std::endl;
            //renderer.pauseMusicStream();
            //renderer.resumeMusicStream();
        },
        [&renderer] {
            soundEffectsEnabled = !soundEffectsEnabled;
            std::cout << "Sound Effects " << (soundEffectsEnabled ? "enabled" : "disabled") << std::endl;
            //renderer.playSound();
            //renderer.stopSound();
        },
        [&renderer] {
            const std::vector<std::pair<int, int>> resolutions = {{800, 600}, {1280, 720}, {1920, 1080}};
            currentResolutionIndex = (currentResolutionIndex + 1) % resolutions.size();
            auto [x, y] = resolutions[currentResolutionIndex];
            renderer.setWindowSize(x, y);
            std::cout << "Resolution set to: " << x << "x" << y << std::endl;
        },
        [&renderer]() {
            colorBlindMode = !colorBlindMode;
            std::cout << "Colorblind Mode " << (colorBlindMode ? "enabled" : "disabled") << std::endl;
        }
    };

    for (std::size_t i = 0; i < settingsOptions.size(); ++i) {
        entity_t optionEntity = r.spawn_entity();

        r.add_component(optionEntity, Components::RenderText(settingsOptions[i], 20));
        r.add_component(optionEntity, Position(100, static_cast<float>(150 + i * 40)));
        r.add_component(optionEntity, Components::ColorText({255, 255, 255, 255}));
        r.add_component(optionEntity, Components::ClickableText([i, actions](Registry &) {
            actions[i]();
        }));
        r.add_component(optionEntity, Components::ColorOverText({20, 82, 172, 255}, {255, 255, 255, 255}));
        r.add_component(optionEntity, Components::MouseOverText(false));
    }

    exitButtonCallback(r);
}
