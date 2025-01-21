/*
** EPITECH PROJECT, 2025
** R_Type
** File description:
** Settings scene
*/

#include "Client.hpp"
#include "Zipper.hh"
#include <fstream>
#include <sstream>
#include <fstream>
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
    r.add_component(backButton, Components::ColorOverText({20, 82, 172, 255}, {255, 255, 255, 255}));
    r.add_component(backButton, Components::MouseOverText(false));
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

void storeLocalUsername(const std::string &username) {
    std::ofstream file(USER_CONFIG_FILEPATH);

    if (file.is_open()) {
        file << "username=" << username;
        file.close();
    }
}

void logoutCallback(Registry &r) {
    storeLocalUsername("");
    r.clear_entities();
    createSignForm(r);
}

void exitCallback(Registry &r) {
    r.clear_entities();
    exit(0);
}

std::string getLocalUsername() {
    std::ifstream file(USER_CONFIG_FILEPATH);

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string key;
            if (std::getline(iss, key, '=')) {
                std::string value;
                if (std::getline(iss, value)) {
                    if (key == "username" && !value.empty()) {
                        file.close();
                        return value;
                    }
                }
            }
        }
        file.close();
    }
    return "";
}



void createSignForm(Registry &r) {
    entity_t inputTextTitle = r.spawn_entity();
    constexpr Color white = COLOR_WHITE;
    constexpr Color grey = COLOR_GREY;
    constexpr Color darkBlue = COLOR_DARK_BLUE;
    Graphic::IRenderer &renderer = Client::getInstance().getRenderer();
    std::string enterUsernameText = "Enter your username:";


    // Enter username title
    int textWidth = renderer.measureText(enterUsernameText, 20);
    int x = (WIDTH - renderer.measureText(enterUsernameText, 20)) / 2;
    int y = (HEIGHT - renderer.measureText(enterUsernameText, 20)) / 2;

    r.add_component(inputTextTitle, Components::RenderText(enterUsernameText, 20, true));
    r.add_component(inputTextTitle, Position(x, y));
    r.add_component(inputTextTitle, Components::ColorText(white));
    r.add_component(inputTextTitle, Components::Input(enterUsernameText));
    r.add_component(inputTextTitle, Components::ColorOverText(darkBlue, grey));
    r.add_component(inputTextTitle, Components::MouseOverText(false));


    float inputX = x - textWidth / 2;
    // Input rectangle
    entity_t inputRectangle = r.spawn_entity();
    r.add_component(inputRectangle, Components::Rect(white, textWidth * 2 + 10, 40));
    r.add_component(inputRectangle, Position(inputX, y + 30));
    r.add_component(inputRectangle, Components::Input(enterUsernameText));

    // Input Text
    entity_t inputText = r.spawn_entity();
    r.add_component(inputText, Components::InputText({"", 20, true}));
    r.add_component(inputText, Position(inputX + 10, y +10));
    r.add_component(inputText, Components::Input(enterUsernameText));
    r.add_component(inputText, Components::ColorText(darkBlue));

    // Sign in text button
    const std::string signInText = "Start";
    entity_t signIntButton = r.spawn_entity();
    r.add_component(signIntButton, Components::RenderText(signInText, 20, true));
    r.add_component(signIntButton, Position(WIDTH / 2 - 40, y + 100));
    r.add_component(signIntButton, Components::ColorText(white));
    r.add_component(signIntButton, Components::ColorOverText(darkBlue, grey));
    r.add_component(signIntButton, Components::MouseOverText(false));
    r.add_component(signIntButton, Components::Input(enterUsernameText));
    r.add_component(signIntButton, Components::ClickableText([signInText, enterUsernameText](Registry &r) {
        auto &inputTexts = r.get_components<Components::InputText>();
        auto &inputs = r.get_components<Components::Input>();

        for (auto &&[inputText, input] : Zipper(inputTexts, inputs)) {
            if (input.inputTextTitle == enterUsernameText && !inputText.text.text.empty()) {
                storeLocalUsername(inputText.text.text);
                r.clear_entities();
                createMenuScene(r);

                
            }
        }
    }));
}


std::function<void(int)> makeSound(int soundID) {
    return [soundID](int passedSoundID) {
        Graphic::IRenderer &renderer = Client::getInstance().getRenderer();
        renderer.playSound(soundID);
    };
}

void accessibilityCallback(Registry &r) {
    auto &texts = r.get_components<Components::RenderText>();
    auto &buttons = r.get_components<Components::MouseOverTextSound>();
    auto &colorTexts = r.get_components<Components::ColorText>();

    isAccessibilityOn = !isAccessibilityOn;
    for (auto &&[button, colorText, text] : Zipper(buttons, colorTexts, texts)) {
        if (text.text == "Accessibility") {
            colorText.color = isAccessibilityOn ? Color(COLOR_DARK_BLUE) : Color(COLOR_WHITE);
        }
    }
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
    const std::vector<std::function<void(Registry &r)>> callbacks = {lobbyCallback, leaderBoardCallback, settingsCallback, creditsCallback, logoutCallback, exitCallback, accessibilityCallback};

    for (std::size_t i = 0; i < NB_MENU_BUTTONS; i++) {
        entity_t button = r.spawn_entity();

        r.add_component(button, Components::RenderText(menuButtonsTitles[i], 20, true));
        if (menuButtonsTitles[i] == "Accessibility") {
            r.add_component(button, Position(WIDTH - 200, HEIGHT - 50));
        } else {
            r.add_component(button, Position(50, static_cast<float>(150 + i * 50)));
        }

        r.add_component(button, Components::ColorText(grey));
        r.add_component(button, Components::ClickableText(callbacks[i]));
        r.add_component(button, Components::ColorOverText(darkBlue, grey));
        r.add_component(button, Components::MouseOverText(false));
        r.add_component(button, Components::MouseOverTextSound(makeSound(i + 4), i + 4));
    }

    const std::string username = getLocalUsername();
    entity_t usernameEntity = r.spawn_entity();
    r.add_component(usernameEntity, Components::RenderText("Welcome " + username, 20, true));
    r.add_component(usernameEntity, Position(50, 100));
    r.add_component(usernameEntity, Components::ColorText(white));
    r.add_component(usernameEntity, Components::MouseOverText(false));
}
