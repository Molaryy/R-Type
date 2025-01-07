#include "Menu.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

/**
 * @brief Construct a new Menu:: Menu object
 * 
 * @param rl 
 */
Menu::Menu(rtype::RayLib &rl) : selectedOption(0), currentMenu(MAIN_MENU) {
    lobbies = {
        Lobby("Lobby 1"),
        Lobby("Lobby 2"),
        Lobby("Lobby 3")
    };
    menuItems = {
        {"Play", "Join a lobby to start playing.", [this, &rl]() { showLobbies(rl); }},
        {"Leaderboard", "View the top scores.", [this, &rl]() { showLeaderboard(rl); }},
        {"Settings", "Change settings.", [this, &rl]() { showSettings(rl); }},
        {"Credits", "View credits.", [this, &rl]() { showCredits(rl); }},
        {"Quit", "Exit the game.", [this]() { quitGame(); }}
    };
}

void Menu::showLobbies(rtype::RayLib &rl)
{
    static int selectedLobby = 0;

    while (!rl.windowShouldClose()) {
        rl.beginDrawing();
        rl.clearBackground(BLACK);
        rl.drawText("Select a Lobby", 50, 50, 40, WHITE);

        for (size_t i = 0; i < lobbies.size(); ++i) {
            Color color = (i == selectedLobby) ? DARKBLUE : GRAY;
            std::string lobbyInfo = lobbies[i].name + " - Players: " + std::to_string(lobbies[i].players.size());
            rl.drawText(lobbyInfo.c_str(), 100, 150 + i * 40, 20, color);
        }
        rl.drawText("Press ENTER to join", 50, 550, 20, LIGHTGRAY);
        rl.endDrawing();

        if (rl.isKeyPressed(KEY_DOWN)) selectedLobby = (selectedLobby + 1) % lobbies.size();
        if (rl.isKeyPressed(KEY_UP)) selectedLobby = (selectedLobby - 1 + lobbies.size()) % lobbies.size();
        if (rl.isKeyPressed(KEY_ENTER)) {
            joinLobby(rl, selectedLobby);
            break;
        }
    }
}

void Menu::joinLobby(rtype::RayLib &rl, int lobbyIndex)
{
    // TODO : Integration with server, lobby à la dure
    Lobby& lobby = lobbies[lobbyIndex];
    std::string playerName = "Player Test" + std::to_string(lobby.players.size() + 1);
    lobby.players.push_back(playerName);

    while (!rl.windowShouldClose()) {
        rl.beginDrawing();
        rl.clearBackground(BLACK);
        rl.drawText(lobby.name.c_str(), 50, 50, 40, WHITE);
        rl.drawText("Players in Lobby:", 50, 100, 30, WHITE);

        for (size_t i = 0; i < lobby.players.size(); ++i) {
            rl.drawText(lobby.players[i].c_str(), 100, 150 + i * 30, 20, GRAY);
        }
        rl.drawText("Press ENTER to start the game", 50, 550, 20, LIGHTGRAY);
        rl.endDrawing();

        if (rl.isKeyPressed(KEY_ENTER)) {
            lobby.gameStarted = true;
            playGame(rl);
            break;
        }
    }
}

/**
 * @brief Get & Sort players by scores
 * 
 * @param filename 
 * @return std::vector<PlayerScore> 
 */
std::vector<PlayerScore> loadScores(const std::string &filename)
{
    std::vector<PlayerScore> scores;
    std::ifstream file(filename);

    if (!file.is_open())
        return scores;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        PlayerScore ps;
        if (iss >> ps.name >> ps.score) {
            scores.push_back(ps);
        }
    }

    file.close();
    std::sort(scores.begin(), scores.end(), [](const PlayerScore &a, const PlayerScore &b) {
        return a.score > b.score;
    });
    return scores;
}

/**
 * @brief Display best players from a save file
 * 
 * @param rl 
 */
void Menu::showLeaderboard(rtype::RayLib &rl)
{
    std::vector<PlayerScore> scores = loadScores("../scores.txt");

    //! TODO: check if scores.txt exists

    while (!rl.windowShouldClose()) {
        rl.beginDrawing();
        rl.clearBackground(BLACK);
        rl.drawText("Leaderboard", 50, 50, 40, WHITE);
        rl.drawText("Top 10 Players", 50, 100, 30, WHITE);
        int yPosition = 180;

        for (size_t i = 0; i < scores.size() && i < 10; ++i) {
            std::string entry = std::to_string(i + 1) + ". " + scores[i].name + " - " + std::to_string(scores[i].score);
            rl.drawText(entry.c_str(), 50, yPosition, 20, DARKBLUE);
            yPosition += 30;
        }
        rl.drawText("Press SPACE to return to Menu", 50, yPosition + 50, 20, WHITE);
        rl.endDrawing();

        if (rl.isKeyPressed(KEY_SPACE))
            break;
    }
}

void Menu::drawSettingsOptions(rtype::RayLib &rl, int selectedOption, int currentFPSIndex, bool musicEnabled, bool soundEffectsEnabled, int currentResolutionIndex, bool colorBlindMode) {
    const char* settingsOptions[] = {
        "Change FPS",
        "Toggle Music",
        "Toggle Sound Effects",
        "Change Resolution",
        "Toggle Colorblind Mode",
        "Back to Menu"
    };

    const int optionsCount = sizeof(settingsOptions) / sizeof(settingsOptions[0]);
    const int maxFPSOptions[] = {30, 60, 120};
    const std::vector<std::pair<int, int>> resolutions = {
        {800, 600}, {1280, 720}, {1920, 1080}
    };


    rl.drawText("Settings", 300, 50, 40, WHITE);

    for (int i = 0; i < optionsCount; i++) {
        Color optionColor = (i == selectedOption) ? DARKBLUE : GRAY;
        rl.drawText(settingsOptions[i], 100, 150 + i * 40, 20, optionColor);
    }

    // for (int i = 0; i <= 3; i++) {
    //     if (maxFPSOptions[i] == 30) {
    //         return (30);
    //     }
    //     if (maxFPSOptions[i] == 60) {
    //         return (60);
    //     }
    //     if (maxFPSOptions[i] == 120) {
    //         return (120);
    //     }
    // }

    rl.drawText(TextFormat("FPS: %d", maxFPSOptions[currentFPSIndex]), 400, 150, 20, LIGHTGRAY);
    rl.drawText(TextFormat("Music: %s", musicEnabled ? "On" : "Off"), 400, 190, 20, LIGHTGRAY);
    rl.drawText(TextFormat("Sound Effects: %s", soundEffectsEnabled ? "On" : "Off"), 400, 230, 20, LIGHTGRAY);
    rl.drawText(TextFormat("Resolution: %dx%d", resolutions[currentResolutionIndex].first, resolutions[currentResolutionIndex].second), 400, 270, 20, LIGHTGRAY);
    rl.drawText(TextFormat("Colorblind Mode: %s", colorBlindMode ? "On" : "Off"), 400, 310, 20, LIGHTGRAY);
    rl.drawText("Press ENTER to select", 50, 550, 20, LIGHTGRAY);
}

/**
 * @brief Handle & display settings
 * 
 * @param rl 
 */
void Menu::showSettings(rtype::RayLib &rl) {
    const int maxFPSOptions[] = {30, 60, 120};
    int currentFPSIndex = 1; // 60 FPS par défaut
    bool musicEnabled = true;
    bool soundEffectsEnabled = true;
    bool colorBlindMode = false;

    const std::vector<std::pair<int, int>> resolutions = {
        {800, 600}, {1280, 720}, {1920, 1080}
    };

    int currentResolutionIndex = 0; // 800x600 par défaut
    int selectedOption = 0;

    const int optionsCount = 6;

    while (!rl.windowShouldClose()) {
        if (rl.isKeyPressed(KEY_DOWN)) selectedOption = (selectedOption + 1) % optionsCount;
        if (rl.isKeyPressed(KEY_UP)) selectedOption = (selectedOption - 1 + optionsCount) % optionsCount;

        if (rl.isKeyPressed(KEY_ENTER)) {
            switch (selectedOption) {
                case 0:
                    currentFPSIndex = (currentFPSIndex + 1) % (sizeof(maxFPSOptions) / sizeof(maxFPSOptions[0]));
                    rl.setTargetFPS(maxFPSOptions[currentFPSIndex]);
                    break;
                case 1:
                    musicEnabled = !musicEnabled;
                    break;
                case 2:
                    soundEffectsEnabled = !soundEffectsEnabled;
                    break;
                case 3:
                    currentResolutionIndex = (currentResolutionIndex + 1) % resolutions.size();
                    rl.setWindowSize(resolutions[currentResolutionIndex].first, resolutions[currentResolutionIndex].second);
                    break;
                case 4:
                    colorBlindMode = !colorBlindMode;
                    break;
                case 5:
                    currentMenu = MAIN_MENU;
                    return;
            }
        }

        rl.beginDrawing();
        rl.clearBackground(BLACK);
        drawSettingsOptions(rl, selectedOption, currentFPSIndex, musicEnabled, soundEffectsEnabled, currentResolutionIndex, colorBlindMode);
        rl.endDrawing();
    }
}

/**
 * @brief Display credits
 * 
 * @param rl 
 */
void Menu::showCredits(rtype::RayLib &rl)
{
    const char* credits[] = {
        "Game Developers:",
        " - Leo OUTMIZGUINE"
        " - Mohammed JBILOU",
        " - Nicolas GILLARD",
        " - Arthaud POUPARD",
        " - Mounia ARJDAL",
    };

    const int creditsCount = sizeof(credits) / sizeof(credits[0]);
    float scrollPosition = 600;
    const float scrollSpeed = 60.0f;

    while (!rl.windowShouldClose()) {
        float frameTime = rl.getFrameTime();
        scrollPosition -= scrollSpeed * frameTime;

        rl.beginDrawing();
        rl.clearBackground(BLACK);

        for (int i = 0; i < creditsCount; i++) {
            float yPosition = scrollPosition + i * 40;
            if (yPosition > -40 && yPosition < 600) {
                rl.drawText(credits[i], 100, yPosition, 20, WHITE);
            }
        }
        rl.drawText("Press SPACE to return to Menu", 450, 10, 20, WHITE);
        rl.endDrawing();

        if (rl.isKeyPressed(KEY_SPACE)) {
            currentMenu = MAIN_MENU;
            break;
        }

        if (scrollPosition + creditsCount * 40 < 0)
            scrollPosition = 600;
    }
}

/**
 * @brief Game main function when section is pressed 
 * 
 * @param rl 
 */
void Menu::playGame(rtype::RayLib &rl) {
    for (Lobby& lobby : lobbies) {
        if (lobby.gameStarted) {
            Game game;
            game.run(rl);
            lobby.gameStarted = false; // Réinitialiser le statut du lobby après la fin du jeu
            return;
        }
    }
    currentMenu = MAIN_MENU;
}

/**
 * @brief Quit game when section is pressed
 * 
 */
void Menu::quitGame()
{
    currentMenu = EXIT;
}

/**
 * @brief Menu main function
 * 
 * @param rl 
 */
void Menu::run(rtype::RayLib &rl) {
    rl.initWindow(800, 600, "R-TYPE");
    rl.setTargetFPS(60);

    while (currentMenu != EXIT && !rl.windowShouldClose()) {
        rl.beginDrawing();
        rl.clearBackground(BLACK);

        rl.drawText("R-TYPE", 50, 50, 40, WHITE);

        for (size_t i = 0; i < menuItems.size(); ++i) {
            Color color = (i == selectedOption) ? DARKBLUE : GRAY;
            rl.drawText(menuItems[i].label.c_str(), 100, 150 + i * 40, 20, color);
        }
        rl.drawText(menuItems[selectedOption].description.c_str(), 50, 550, 20, LIGHTGRAY);
        rl.endDrawing();

        if (rl.isKeyPressed(KEY_DOWN)) selectedOption = (selectedOption + 1) % menuItems.size();
        if (rl.isKeyPressed(KEY_UP)) selectedOption = (selectedOption - 1 + menuItems.size()) % menuItems.size();
        if (rl.isKeyPressed(KEY_ENTER)) menuItems[selectedOption].action();
    }

    rl.closeWindow();
}
