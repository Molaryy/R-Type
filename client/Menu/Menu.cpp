#include "Menu.hpp"
#include <fstream>
#include <sstream>

Menu::Menu(rtype::RayLib &rl) : selectedOption(0), currentMenu(MAIN_MENU)
{
    menuItems = {
        {"New Game", "Start a new game.", [this, &rl]() { playGame(rl); }},
        {"Leaderboard", "View the top scores.", [this, &rl]() { showLeaderboard(rl); }},
        {"Settings", "Change settings.", [this, &rl]() { showSettings(rl); }},
        {"Credits", "View credits.", [this, &rl]() { showCredits(rl); }},
        {"Quit", "Exit the game.", [this]() { quitGame(); }}
    };
}

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

void Menu::showLeaderboard(rtype::RayLib &rl)
{
    std::vector<PlayerScore> scores = loadScores("scores.txt");

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

void Menu::showSettings(rtype::RayLib &rl)
{
    const int maxFPSOptions[] = {30, 60, 120};
    int currentFPSIndex = 1; // 60 FPS défaut
    bool musicEnabled = true;
    bool soundEffectsEnabled = true;
    bool colorBlindMode = false;

    const std::vector<std::pair<int, int>> resolutions = {
        {800, 600}, {1024, 768}, {1280, 720}, {1920, 1080}
    };
    int currentResolutionIndex = 0; // 800x600 défaut
    int selectedOption = 0;

    const char* settingsOptions[] = {
        "Change FPS",
        "Toggle Music",
        "Toggle Sound Effects",
        "Change Resolution",
        "Toggle Colorblind Mode",
        "Back to Menu"
    };

    const int optionsCount = sizeof(settingsOptions) / sizeof(settingsOptions[0]);

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
                    rl.SetWindowSize(resolutions[currentResolutionIndex].first, resolutions[currentResolutionIndex].second);
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
        rl.drawText("Settings", 300, 50, 40, WHITE);

        for (int i = 0; i < optionsCount; i++) {
            Color optionColor = (i == selectedOption) ? DARKBLUE : GRAY;
            rl.drawText(settingsOptions[i], 100, 150 + i * 40, 20, optionColor);
        }

        rl.drawText(TextFormat("FPS: %d", maxFPSOptions[currentFPSIndex]), 400, 150, 20, LIGHTGRAY);
        rl.drawText(TextFormat("Music: %s", musicEnabled ? "On" : "Off"), 400, 190, 20, LIGHTGRAY);
        rl.drawText(TextFormat("Sound Effects: %s", soundEffectsEnabled ? "On" : "Off"), 400, 230, 20, LIGHTGRAY);
        rl.drawText(TextFormat("Resolution: %dx%d", resolutions[currentResolutionIndex].first, resolutions[currentResolutionIndex].second), 400, 270, 20, LIGHTGRAY);
        rl.drawText(TextFormat("Colorblind Mode: %s", colorBlindMode ? "On" : "Off"), 400, 310, 20, LIGHTGRAY);
        rl.drawText("Press ENTER to select, UP/DOWN to navigate", 50, 550, 20, LIGHTGRAY);
        rl.endDrawing();
    }
}

void Menu::showCredits(rtype::RayLib &rl)
{
    const char* credits[] = {
        "Game Developers:",
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

void Menu::playGame(rtype::RayLib &rl)
{
    Game game;
    game.run(rl);
}

void Menu::quitGame()
{
    currentMenu = EXIT;
}

void Menu::run(rtype::RayLib &rl)
{
    rl.initWindow(800, 600, "Menu");
    rl.setTargetFPS(60);

    while (currentMenu != EXIT && !rl.windowShouldClose()) {
        rl.beginDrawing();
        rl.clearBackground(BLACK);

        rl.drawText("Main Menu", 50, 50, 40, WHITE);

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
