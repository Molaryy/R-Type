/*
** EPITECH PROJECT, 2025
** R_Type
** File description:
** Settings scene
*/

#include "Scenes.hpp"


void newGameCallback(Registry &r)
{
    r.clear_enities();
    entity_t e = r.spawn_entity();

    r.add_component(e, Components::RenderText("Player", 50, 50, 40));
    r.add_component(e, Components::ColorText(COLOR_WHITE));
}

void leaderBoardCallback(Registry &r) {
    std::cout << "leader callback" <<  std::endl;
}



void creditsCallback(Registry &r) {
    std::cout << "credits callback" <<  std::endl;
}

void exitCallback(Registry &r) {
    std::cout << "exit callback" <<  std::endl;
}

void createMenuScene(Registry &registry) {
    entity_t e = registry.spawn_entity();
    Color white = {255, 255, 255, 255};

    registry.add_component(e, Components::RenderText("R-TYPE", 50, 50, 40));
    registry.add_component(e, Components::ColorText(white));
    std::vector<std::string> titles = {"New Game", "Leaderboard", "Settings", "Credits", "Exit"};
    std::vector<std::function<void(Registry &r)>> callbacks = {newGameCallback, leaderBoardCallback, settingsCallback, creditsCallback, exitCallback};

    for (std::size_t i = 0; i < NB_MENU_BUTTONS; i++)
    {
        entity_t button = registry.spawn_entity();
        Color grey = COLOR_GREY;
        Color darkBlue = COLOR_DARK_BLUE;
    
        registry.add_component(button, Components::RenderText(titles[i], 100, 150 + i * 50, 20));
        registry.add_component(button, Components::ColorText(grey));
        registry.add_component(button, Components::ClickableText(callbacks[i]));
        registry.add_component(button, Components::ColorOverText(darkBlue, grey, false));
    }
}
