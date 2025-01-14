/*
** EPITECH PROJECT, 2025
** R_Type
** File description:
** Settings scene
*/


#include "Components.hpp"
#include "Scenes.hpp"


void newGameCallback(Registry &r) {
    r.clear_entities();
    const entity_t e = r.spawn_entity();

    r.add_component(e, Components::RenderText("Player", 50, 50, 40));
    r.add_component(e, Components::ColorText(COLOR_WHITE));
}

void leaderBoardCallback(Registry &r) {
    std::cout << "leader callback" << std::endl;
}


void creditsCallback(Registry &r) {
    std::cout << "credits callback" << std::endl;
}

void exitCallback(Registry &r) {
    std::cout << "exit callback" << std::endl;
}

void createMenuScene(Registry &registry) {
    const entity_t e = registry.spawn_entity();
    constexpr Color white = COLOR_WHITE;

    registry.add_component(e, Components::RenderText("R-TYPE", 50, 50, 40));
    registry.add_component(e, Components::ColorText(white));
    const std::vector<std::string> titles = {"New Game", "Leaderboard", "Settings", "Credits", "Exit"};
    const std::vector<std::function<void(Registry &r)>> callbacks = {newGameCallback, leaderBoardCallback, settingsCallback, creditsCallback, exitCallback};

    for (std::size_t i = 0; i < NB_MENU_BUTTONS; i++) {
        entity_t button = registry.spawn_entity();
        constexpr Color grey = COLOR_GREY;
        constexpr Color darkBlue = COLOR_DARK_BLUE;

        registry.add_component(button, Components::RenderText(titles[i], 100, static_cast<int>(150 + i * 50), 20));
        registry.add_component(button, Components::ColorText(grey));
        registry.add_component(button, Components::ClickableText(callbacks[i]));
        registry.add_component(button, Components::ColorOverText(darkBlue, grey, false));
    }
}
