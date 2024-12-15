#include "raylib/raylib.hh"
#include <functional>
#include <utility>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>

typedef enum {
    MAIN_MENU,
    PLAY_GAME,
    LEADERBOARD,
    SETTINGS,
    CREDITS,
    EXIT
} MenuState;

struct Missile {
    Vector2 position;
    float speed;
    bool active;
};

void PlayGame(MenuState &currentMenu, rtype::RayLib &rl);
void ShowLeaderboard(MenuState &currentMenu, rtype::RayLib &rl);
void ShowSettings(MenuState &currentMenu, rtype::RayLib &rl);
void ShowCredits(MenuState &currentMenu, rtype::RayLib &rl);
void QuitGame(MenuState &currentMenu, rtype::RayLib &rl);

int main()
{
    rtype::RayLib rl;

    rl.initWindow(800, 600, "R-Type");
    rl.setTargetFPS(60);

    MenuState currentMenu = MAIN_MENU;

    std::vector<std::pair<const char*, const char*>> menuItems = {
        {"New Game", "Start a new game."},
        {"Leaderboard", "View the top scores and player rankings."},
        {"Settings", "Change game settings like sound or graphics."},
        {"Credits", "See the credits for the team behind the game."},
        {"Quit", "Exit the game."}
    };

    int menuOptionCount = menuItems.size();
    int selectedOption = 0;

    std::function<void(MenuState &, rtype::RayLib &)> actions[] = {
        PlayGame,
        ShowLeaderboard,
        ShowSettings,
        ShowCredits,
        QuitGame
    };

    Font menuFont = rl.getFontDefault();

    while (!rl.windowShouldClose()) {
        if (rl.isKeyPressed(KEY_DOWN)) selectedOption = (selectedOption + 1) % menuOptionCount;
        if (rl.isKeyPressed(KEY_UP)) selectedOption = (selectedOption - 1 + menuOptionCount) % menuOptionCount;

        rl.beginDrawing();
        rl.clearBackground(BLACK);

        rl.drawTextEx(menuFont, "R-TYPE", (Vector2){50, 50}, 40, 2, WHITE);

        for (int i = 0; i < menuOptionCount; i++) {
            Color optionColor = (i == selectedOption) ? DARKBLUE : GRAY;
            rl.drawText(menuItems[i].first, 100, 150 + i * 40, 20, optionColor);
        }

        rl.drawText(menuItems[selectedOption].second, 50, 550, 20, LIGHTGRAY);

        if (rl.isKeyPressed(KEY_ENTER))
            actions[selectedOption](currentMenu, rl);

        rl.endDrawing();
    }

    rl.closeWindow();
    return 0;
}

struct PlayerScore {
    std::string name;
    int score;
};

std::vector<PlayerScore> loadScores(const std::string &filename) {
    std::vector<PlayerScore> scores;
    std::ifstream file(filename);

    if (!file.is_open()) {
        return scores;
    }

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

void ShowLeaderboard(MenuState &currentMenu, rtype::RayLib &rl) {
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

        if (rl.isKeyPressed(KEY_SPACE)) {
            currentMenu = MAIN_MENU;
            break;
        }
    }
}

void ShowSettings(MenuState &currentMenu, rtype::RayLib &rl)
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

void ShowCredits(MenuState &currentMenu, rtype::RayLib &rl)
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

        if (scrollPosition + creditsCount * 40 < 0) {
            scrollPosition = 600;
        }
    }
}

void QuitGame(MenuState &currentMenu, rtype::RayLib &rl)
{
    currentMenu = EXIT;
}

void PlayGame(MenuState &currentMenu, rtype::RayLib &rl)
{
    rl.initWindow(800, 600, "R-TYPE");
    rl.setTargetFPS(60);

    Vector2 playerPosition = {50.0f, 300.0f};
    float playerSpeed = 300.0f;

    std::vector<Missile> missiles;
    float missileSpeed = 400.0f;

    struct Obstacle {
        Vector2 position;
        int hitsRemaining;
        bool active;
    };

    std::vector<Obstacle> obstacles;
    float obstacleSpawnTimer = 0.0f;
    float obstacleSpawnInterval = 2.0f;

    int score = 0;

    while (!rl.windowShouldClose()) {
        float frameTime = rl.getFrameTime();

        if (rl.isKeyDown(KEY_RIGHT)) playerPosition.x += playerSpeed * frameTime;
        if (rl.isKeyDown(KEY_LEFT)) playerPosition.x -= playerSpeed * frameTime;
        if (rl.isKeyDown(KEY_UP)) playerPosition.y -= playerSpeed * frameTime;
        if (rl.isKeyDown(KEY_DOWN)) playerPosition.y += playerSpeed * frameTime;

        playerPosition.x = std::max(0.0f, std::min(750.0f, playerPosition.x));
        playerPosition.y = std::max(0.0f, std::min(550.0f, playerPosition.y));

        if (rl.isKeyPressed(KEY_SPACE)) {
            Missile missile = {{playerPosition.x + 50.0f, playerPosition.y + 20.0f}, missileSpeed, true};
            missiles.push_back(missile);
        }

        for (auto &missile : missiles) {
            if (missile.active) {
                missile.position.x += missile.speed * frameTime;
                if (missile.position.x > 800) missile.active = false;
            }
        }

        missiles.erase(std::remove_if(missiles.begin(), missiles.end(), [](const Missile &m) { return !m.active; }), missiles.end());

        obstacleSpawnTimer += frameTime;
        if (obstacleSpawnTimer >= obstacleSpawnInterval) {
            Obstacle obstacle = {{800.0f, static_cast<float>(rand() % 550)}, 3, true};
            obstacles.push_back(obstacle);
            obstacleSpawnTimer = 0.0f;
        }

        for (auto &obstacle : obstacles) {
            if (obstacle.active) {
                obstacle.position.x -= 100.0f * frameTime;
                if (obstacle.position.x < 0) {
                    obstacle.active = false;
                }
            }
        }

        for (auto &obstacle : obstacles) {
            if (!obstacle.active) continue;

            for (auto &missile : missiles) {
                if (!missile.active) continue;

                if (CheckCollisionRecs({missile.position.x, missile.position.y, 20, 10},
                {obstacle.position.x, obstacle.position.y, 50, 50})) {
                    missile.active = false;
                    obstacle.hitsRemaining--;

                    if (obstacle.hitsRemaining <= 0) {
                        obstacle.active = false;
                        score += 10;
                    }
                }
            }
        }

        obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [](const Obstacle &o) { return !o.active; }), obstacles.end());

        rl.beginDrawing();
        rl.clearBackground(BLACK);

        rl.drawRectangle(playerPosition.x, playerPosition.y, 50, 50, DARKBLUE);

        for (const auto &missile : missiles) {
            if (missile.active) rl.drawRectangle(missile.position.x, missile.position.y, 20, 10, GOLD);
        }

        for (const auto &obstacle : obstacles) {
            if (obstacle.active) {
                rl.drawRectangle(obstacle.position.x, obstacle.position.y, 50, 50, RED);
                rl.drawText(TextFormat("%d", obstacle.hitsRemaining), obstacle.position.x + 15, obstacle.position.y + 15, 20, WHITE);
            }
        }

        rl.drawText(TextFormat("Score: %d", score), 10, 10, 20, WHITE);
        rl.drawText("Use arrows to move, SPACE to shoot", 10, 40, 20, LIGHTGRAY);

        rl.endDrawing();
    }

    rl.closeWindow();
}

