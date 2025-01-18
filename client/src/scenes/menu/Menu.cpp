/*
** EPITECH PROJECT, 2025
** R_Type
** File description:
** Settings scene
*/

#include "Components.hpp"
#include "Scenes.hpp"
#include "Client.hpp"
#include <fstream>
#include <sstream>

void exitButtonCallback(Registry &r)
{
    entity_t backButton = r.spawn_entity();
    r.add_component(backButton, Components::RenderText("Back to Menu", 50, 500, 20));
    r.add_component(backButton, Components::ColorText({255, 255, 255, 255}));
    r.add_component(backButton, Components::ClickableText([](Registry &r) {
        createMenuScene(r);
    }));
    r.add_component(backButton, Components::ColorOverText({20, 82, 172, 255}, {255, 255, 255, 255}, false));
}

void leaderBoardCallback(Registry &r)
{
    r.clear_entities();

    const entity_t title = r.spawn_entity();
    r.add_component(title, Components::RenderText("Leaderboard", 50, 50, 40));
    r.add_component(title, Components::ColorText({255, 255, 255, 255}));

    exitButtonCallback(r);
}

void creditsCallback(Registry &r) {
    r.clear_entities();

    const entity_t titleEntity = r.spawn_entity();
    r.add_component(titleEntity, Components::RenderText("CREDITS", 50, 50, 40));
    r.add_component(titleEntity, Components::ColorText({255, 255, 255, 255}));

    const std::vector<std::string> credits = {
        "Game Developers:",
        " - Leo OUTMIZGUINE",
        " - Mohammed JBILOU",
        " - Nicolas GILLARD",
        " - Arthaud POUPARD",
        " - Mounia ARJDAL",
    };

    int yPosition = 150;

    for (const auto &line : credits) {
        entity_t lineEntity = r.spawn_entity();
        r.add_component(lineEntity, Components::RenderText(line, 100, yPosition, 20));
        r.add_component(lineEntity, Components::ColorText({255, 255, 255, 255}));
        yPosition += 30;
    }
    exitButtonCallback(r);
}

void exitCallback(Registry &r) {
    r.clear_entities();
    exit(0); // TODO handle it properly
}

void createMenuScene(Registry &r) {
    r.clear_entities();

    const entity_t e = r.spawn_entity();

    constexpr Color white = COLOR_WHITE;
    constexpr Color grey = COLOR_GREY;
    constexpr Color darkBlue = COLOR_DARK_BLUE;

    r.add_component(e, Components::RenderText("R-TYPE", 50, 50, 40));
    r.add_component(e, Components::ColorText(white));
    const std::vector<std::string> titles = {"Play", "Leaderboard", "Settings", "Credits", "Exit"};
    const std::vector<std::function<void(Registry &r)>> callbacks = {lobbyCallback, leaderBoardCallback, settingsCallback, creditsCallback, exitCallback};

    for (std::size_t i = 0; i < NB_MENU_BUTTONS; i++) {
        entity_t button = r.spawn_entity();

        r.add_component(button, Components::RenderText(titles[i], 100, static_cast<int>(150 + i * 50), 20));
        r.add_component(button, Components::ColorText(grey));
        r.add_component(button, Components::ClickableText(callbacks[i]));
        r.add_component(button, Components::ColorOverText(darkBlue, grey, false));
    }
}
