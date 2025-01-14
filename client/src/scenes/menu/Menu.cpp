/*
** EPITECH PROJECT, 2025
** R_Type
** File description:
** Settings scene
*/


#include "Components.hpp"
#include "Scenes.hpp"
#include <fstream>
struct PlayerScore {
    std::string name;
    int score;
};

std::vector<PlayerScore> loadScores(const std::string &filename)
{
    // std::vector<PlayerScore> scores;
    // std::ifstream file(filename);

    // if (!file.is_open())
    //     return scores;

    // std::string line;
    // while (std::getline(file, line)) {
    //     std::istringstream iss(line);
    //     PlayerScore ps;
    //     if (iss >> ps.name >> ps.score) {
    //         scores.push_back(ps);
    //     }
    // }

    // file.close();
    // std::sort(scores.begin(), scores.end(), [](const PlayerScore &a, const PlayerScore &b) {
    //     return a.score > b.score;
    // });
    // return scores;
}

void leaderBoardCallback(Registry &r)
{
    r.clear_enities();

    entity_t title = r.spawn_entity();
    r.add_component(title, Components::RenderText("Leaderboard", 50, 50, 40));
    r.add_component(title, Components::ColorText({255, 255, 255, 255}));

    // TODO retrieve scores from file
    // TODO function to save scores when game ends

    // std::vector<PlayerScore> scores = loadScores("scores.txt");
    // int yPosition = 100;

    // for (size_t i = 0; i < scores.size() && i < 10; ++i) {
    //     entity_t scoreEntity = r.spawn_entity();
    //     std::string entry = std::to_string(i + 1) + ". " + scores[i].name + " - " + std::to_string(scores[i].score);

    //     r.add_component(scoreEntity, Components::RenderText(entry, 50, yPosition, 20));
    //     r.add_component(scoreEntity, Components::ColorText({255, 255, 255, 255}));
    //     yPosition += 30;
    // }

    // entity_t backButton = r.spawn_entity();
    // r.add_component(backButton, Components::RenderText("Press SPACE to return to Menu", 50, yPosition + 50, 20));
    // r.add_component(backButton, Components::ColorText({255, 255, 255, 255}));
    // r.add_component(backButton, Components::ClickableText([](Registry &r) {
    //     createMenuScene(r);
    // }));
}

void creditsCallback(Registry &r)
{
    r.clear_enities();

    entity_t titleEntity = r.spawn_entity();
    r.add_component(titleEntity, Components::RenderText("CREDITS", 100, 50, 40));
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

    // entity_t backButton = r.spawn_entity();
    // r.add_component(backButton, Components::RenderText("Press SPACE to return to Menu", 100, yPosition + 20, 20));
    // r.add_component(backButton, Components::ColorText({255, 255, 255, 255}));
    // r.add_component(backButton, Components::ClickableText([](Registry &r) {
    //     createMenuScene(r);
    // }));
}


void exitCallback(Registry &r)
{
    r.clear_enities();
}

void createMenuScene(Registry &r)
{
    r.clear_enities();

    entity_t e = r.spawn_entity();
    Color white = {255, 255, 255, 255};

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
