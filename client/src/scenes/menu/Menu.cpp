/*
** EPITECH PROJECT, 2025
** R_Type
** File description:
** Settings scene
*/

#include "Client.hpp"

#include "Components.hh"
#include "Components.hpp"
#include "IRenderer.hpp"
#include "Scenes.hpp"
#include "LeaderBoard.hpp"

void exitButtonCallback(Registry &r)
{
    const entity_t backButton = r.spawn_entity();
    r.add_component(backButton, Components::RenderText("Back to Menu", 20));
    r.add_component(backButton, Position(50, 500));
    r.add_component(backButton, Components::ColorText({255, 255, 255, 255}));
    r.add_component(backButton, Components::ClickableText([](Registry &reg) {
        createMenuScene(reg);
    }));
    r.add_component(backButton, Components::ColorOverText({20, 82, 172, 255}, {255, 255, 255, 255}, false));
}

void leaderBoardCallback(Registry &r) {
    r.clear_entities();

    const entity_t title = r.spawn_entity();
    r.add_component(title, Components::RenderText("Leaderboard", 40, true));
    r.add_component(title, Position(250, 30));
    r.add_component(title, Components::ColorText({255, 255, 255, 255}));

    LeaderBoard leaderboard;
    auto [names, scores] = leaderboard.getScoreboard();
    for (std::size_t i = 0; i < SCOREBOARD_SIZE; ++i)
    {
        if (std::strlen(names[i]) == 0 && scores[i] == 0) {
            break;
        }
        const entity_t nameEntity = r.spawn_entity();
        const entity_t scoreEntity = r.spawn_entity();
        const std::string scoreboard_name = names[i];
        const std::size_t scoreboard_scores = scores[i];
        r.add_component(nameEntity, Components::RenderText(scoreboard_name, 30, true));
        r.add_component(nameEntity, Position(200, static_cast<float>(100 + i * 40)));
        r.add_component(nameEntity, Components::ColorText({255, 255, 255, 255}));
        r.add_component(scoreEntity, Components::RenderText(std::to_string(scoreboard_scores), 30, true));
        r.add_component(scoreEntity, Position(500, static_cast<float>(100 + i * 40)));
        r.add_component(scoreEntity, Components::ColorText({255, 255, 255, 255}));
    }
    exitButtonCallback(r);
}

void creditsCallback(Registry &r) {
    r.clear_entities();

    const entity_t titleEntity = r.spawn_entity();
    r.add_component(titleEntity, Components::RenderText("CREDITS", 40, true));
    r.add_component(titleEntity, Position(100, 50));
    r.add_component(titleEntity, Components::ColorText({255, 255, 255, 255}));

    const std::vector<std::string> credits = {
        "Game Developers:",
        " - Leo OUTMIZGUINE",
        " - Mohammed JBILOU",
        " - Nicolas GILLARD",
        " - Arthaud POUPARD",
        " - Mounia ARJDAL",
    };

    float yPosition = 150;

    for (const auto &line : credits) {
        entity_t lineEntity = r.spawn_entity();
        r.add_component(lineEntity, Components::RenderText(line, 20, true));
        r.add_component(lineEntity, Position(100, yPosition));
        r.add_component(lineEntity, Components::ColorText({255, 255, 255, 255}));
        yPosition += 30;
    }
    exitButtonCallback(r);
}

void exitCallback(Registry &r) {
    r.clear_entities();
    exit(0);
}

void createMenuScene(Registry &r) {
    r.clear_entities();

    const entity_t e = r.spawn_entity();

    constexpr Color white = COLOR_WHITE;
    constexpr Color grey = COLOR_GREY;
    constexpr Color darkBlue = COLOR_DARK_BLUE;

    r.add_component(e, Components::RenderText("R-TYPE", 40, true));
    r.add_component(e, Position(50, 50));
    r.add_component(e, Components::ColorText(white));
    const std::vector<std::string> titles = {"Play", "Leaderboard", "Settings", "Credits", "Exit"};
    const std::vector<std::function<void(Registry &r)>> callbacks = {lobbyCallback, leaderBoardCallback, settingsCallback, creditsCallback, exitCallback};


    for (std::size_t i = 0; i < NB_MENU_BUTTONS; i++) {
        entity_t button = r.spawn_entity();

        r.add_component(button, Components::RenderText(titles[i], 20, true));
        r.add_component(button, Position(100.0f, static_cast<float>(150 + i * 50)));
        r.add_component(button, Components::ColorText(grey));
        r.add_component(button, Components::ClickableText(callbacks[i]));
        r.add_component(button, Components::ColorOverText(darkBlue, grey, false));
    }

    //Graphic::IRenderer &renderer = Client::getInstance().getRenderer();
    //entity_t pop_up = r.spawn_entity();
    //
    //std::string text = "Enter your username:";
    //r.add_component(pop_up, Components::RenderText(text, 20, true));
    //int textSie = renderer.measureText(text, 20);
    //int x = (WIDTH - renderer.measureText(text, 20)) / 3;
    //int y = (HEIGHT - renderer.measureText(text, 20)) / 2;
    //std::cout << "X: " << x << " Y: " << y << std::endl;
    //r.add_component(pop_up, Position(x, y));
    //r.add_component(pop_up, Components::ColorText(white));
    //r.add_component(pop_up, Components::ColorOverText(darkBlue, grey, false));
}
