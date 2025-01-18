/*
** EPITECH PROJECT, 2025
** R_Type
** File description:
** Settings scene
*/


#include "Components.hpp"
#include "Components.hh"
#include "Scenes.hpp"

struct PlayerScore {
    std::string name;
    int score;
};

std::vector<PlayerScore> loadScores(const std::string &filename) {
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
    return {};
}

void leaderBoardCallback(Registry &r) {
    r.clear_entities();

    const entity_t title = r.spawn_entity();
    r.add_component(title, Components::RenderText("Leaderboard", 40));
    r.add_component(title, Position(50, 50));
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

void creditsCallback(Registry &r) {
    r.clear_entities();

    const entity_t titleEntity = r.spawn_entity();
    r.add_component(titleEntity, Components::RenderText("CREDITS", 40));
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

    int yPosition = 150;
    for (const auto &line : credits) {
        entity_t lineEntity = r.spawn_entity();
        r.add_component(lineEntity, Components::RenderText(line,  20));
        r.add_component(lineEntity, Position(100, yPosition));
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


void exitCallback(Registry &r) {
    r.clear_entities();
}

void createMenuScene(Registry &r) {
    r.clear_entities();

    const entity_t e = r.spawn_entity();

    constexpr Color white = COLOR_WHITE;
    constexpr Color grey = COLOR_GREY;
    constexpr Color darkBlue = COLOR_DARK_BLUE;

    r.add_component(e, Components::RenderText("R-TYPE", 40));
    r.add_component(e, Position(50, 50));
    r.add_component(e, Components::ColorText(white));
    const std::vector<std::string> titles = {"Play", "Leaderboard", "Settings", "Credits", "Exit"};
    const std::vector<std::function<void(Registry &r)>> callbacks = {lobbyCallback, leaderBoardCallback, settingsCallback, creditsCallback, exitCallback};

    for (std::size_t i = 0; i < NB_MENU_BUTTONS; i++) {
        entity_t button = r.spawn_entity();

        r.add_component(button, Components::RenderText(titles[i], 20));
        r.add_component(button, Position(100.0f, static_cast<float>(150 + i * 50)));
        r.add_component(button, Components::ColorText(grey));
        r.add_component(button, Components::ClickableText(callbacks[i]));
        r.add_component(button, Components::ColorOverText(darkBlue, grey, false));
    }
    entity_t pop_up = r.spawn_entity();

    r.add_component(pop_up, Components::RenderText("Enter your username:", 20));
    r.add_component(pop_up, Position(100, 500));
    r.add_component(pop_up, Components::ColorText(white));
}
