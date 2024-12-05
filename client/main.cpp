#include "../raylib/raylib.hh"
#include <functional>

typedef enum {
    MAIN_MENU,
    PLAY_GAME,
    LEADERBOARD,
    SETTINGS,
    CREDITS,
    EXIT
} MenuState;

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
    int selectedOption = 0;

    // STD::PAIR here ?
    const char *options[] = {
        "New Game",
        "Leaderboard",
        "Settings",
        "Credits",
        "Quit"
    };

    // STD::PAIR here ?
    const char *menuDescriptions[] = {
        "Start a new game.",
        "View the top scores and player rankings.",
        "Change game settings like sound or graphics.",
        "See the credits for the team behind the game.",
        "Exit the game."
    };

    int menuOptionCount = sizeof(options) / sizeof(options[0]);

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
            rl.drawText(options[i], 100, 150 + i * 40, 20, optionColor);
        }

        rl.drawText(menuDescriptions[selectedOption], 50, 550, 20, LIGHTGRAY);

        if (rl.isKeyPressed(KEY_ENTER))
            actions[selectedOption](currentMenu, rl);

        rl.endDrawing();
    }

    rl.closeWindow();
    return 0;
}

void PlayGame(MenuState &currentMenu, rtype::RayLib &rl) {
    while (!rl.windowShouldClose()) {
        rl.beginDrawing();
        rl.clearBackground(DARKBLUE);
        rl.drawText("Playing the game...", 300, 250, 30, WHITE);
        rl.drawText("Press SPACE to return to Menu", 250, 300, 20, WHITE);
        rl.endDrawing();
        if (rl.isKeyPressed(KEY_SPACE)) {
            currentMenu = MAIN_MENU;
            break;
        }
    }
}

void ShowLeaderboard(MenuState &currentMenu, rtype::RayLib &rl) {
    while (!rl.windowShouldClose()) {
        rl.beginDrawing();
        rl.clearBackground(DARKBLUE);
        rl.drawText("Leaderboard Section", 300, 250, 30, WHITE);
        rl.drawText("Press SPACE to return to Menu", 250, 300, 20, WHITE);
        rl.endDrawing();
        if (rl.isKeyPressed(KEY_SPACE)) {
            currentMenu = MAIN_MENU;
            break;
        }
    }
}

void ShowSettings(MenuState &currentMenu, rtype::RayLib &rl) {
    while (!rl.windowShouldClose()) {
        rl.beginDrawing();
        rl.clearBackground(DARKBLUE);
        rl.drawText("Settings Section", 300, 250, 30, WHITE);
        rl.drawText("Press SPACE to return to Menu", 250, 300, 20, WHITE);
        rl.endDrawing();
        if (rl.isKeyPressed(KEY_SPACE)) {
            currentMenu = MAIN_MENU;
            break;
        }
    }
}

void ShowCredits(MenuState &currentMenu, rtype::RayLib &rl) {
    while (!rl.windowShouldClose()) {
        rl.beginDrawing();
        rl.clearBackground(DARKBLUE);
        rl.drawText("Credits Section", 300, 250, 30, WHITE);
        rl.drawText("Press SPACE to return to Menu", 250, 300, 20, WHITE);
        rl.endDrawing();
        if (rl.isKeyPressed(KEY_SPACE)) {
            currentMenu = MAIN_MENU;
            break;
        }
    }
}

void QuitGame(MenuState &currentMenu, rtype::RayLib &rl) {
    currentMenu = EXIT;
}
