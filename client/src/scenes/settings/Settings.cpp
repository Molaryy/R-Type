/*
** EPITECH PROJECT, 2025
** R_Type
** File description:
** Settings scene
*/

#include "Client.hpp"
#include "Components.hpp"
#include "Scenes.hpp"

void settingsCallback(Registry &r)
{
    r.clear_entities();
    entity_t e = r.spawn_entity();

    auto &client = Client::getInstance();

    r.add_component(e, Components::RenderText("Settings", 50, 50, 40));
    r.add_component(e, Components::ColorText({255, 255, 255, 255}));

    const std::vector<std::string> settingsOptions = {
        "Change FPS",
        "Toggle Music",
        "Toggle Sound Effects",
        "Change Resolution",
        "Toggle Colorblind Mode",
    };

    std::vector<std::function<void()>> actions = {
        [&client]() { client.changeFPS(); },
        [&client]() { client.toggleMusic(); },
        [&client]() { client.toggleSoundEffects(); },
        [&client]() { client.changeResolution(); },
        [&client]() { client.toggleColorBlindMode(); },
    };

    for (std::size_t i = 0; i < settingsOptions.size(); ++i) {
        entity_t optionEntity = r.spawn_entity();
        r.add_component(optionEntity, Components::RenderText(settingsOptions[i], 100, static_cast<int>(150 + i * 40), 20));
        r.add_component(optionEntity, Components::ColorText({255, 255, 255, 255}));
        r.add_component(optionEntity, Components::ClickableText([i, actions](Registry &) {
            actions[i]();
        }));
        r.add_component(optionEntity, Components::ColorOverText({20, 82, 172, 255}, {255, 255, 255, 255}, false));
    }

    exitButtonCallback(r);
}
