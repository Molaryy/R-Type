#include <raylib.h>
#include <functional>

typedef enum {
    MAIN_MENU,
    PLAY_GAME,
    LEADERBOARD,
    SETTINGS,
    CREDITS,
    EXIT
} MenuState;

void PlayGame(MenuState &currentMenu);
void ShowLeaderboard(MenuState &currentMenu);
void ShowSettings(MenuState &currentMenu);
void ShowCredits(MenuState &currentMenu);
void QuitGame(MenuState &currentMenu);

int main()
{
    InitWindow(800, 600, "R-Type");
    SetTargetFPS(60);

    MenuState currentMenu = MAIN_MENU;
    int selectedOption = 0;

    // change to STD::PAIR here?
    const char *options[] = {
        "New Game",
        "Leaderboard",
        "Settings",
        "Credits",
        "Quit"
    };

    const char *menuDescriptions[] = {
        "Start a new game.",
        "View the top scores and player rankings.",
        "Change game settings like sound or graphics.",
        "See the credits for the team behind the game.",
        "Exit the game."
    };

    int menuOptionCount = sizeof(options) / sizeof(options[0]);

    std::function<void(MenuState &)> actions[] = {
        PlayGame,
        ShowLeaderboard,
        ShowSettings,
        ShowCredits,
        QuitGame
    };

    Font menuFont = GetFontDefault();

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_DOWN)) selectedOption = (selectedOption + 1) % menuOptionCount;
        if (IsKeyPressed(KEY_UP)) selectedOption = (selectedOption - 1 + menuOptionCount) % menuOptionCount;

        BeginDrawing();
        ClearBackground(BLACK);

        DrawTextEx(menuFont, "R-TYPE", (Vector2){ 50, 50 }, 40, 2, WHITE);

        for (int i = 0; i < menuOptionCount; i++) {
            Color optionColor = (i == selectedOption) ? DARKBLUE : GRAY;
            DrawText(
                options[i],
                100, 150 + i * 40,
                20,
                optionColor
            );
        }

        DrawText(
            menuDescriptions[selectedOption],
            50,
            550,
            20,
            LIGHTGRAY
        );

        if (IsKeyPressed(KEY_ENTER))
            actions[selectedOption](currentMenu);

        EndDrawing();
    }

    CloseWindow();
}

void PlayGame(MenuState &currentMenu) {
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKBLUE);
        DrawText("Playing the game...", 300, 250, 30, WHITE);
        DrawText("Press SPACE to return to Menu", 250, 300, 20, WHITE);
        EndDrawing();
        if (IsKeyPressed(KEY_SPACE)) {
            currentMenu = MAIN_MENU;
            break;
        }
    }
}

void ShowLeaderboard(MenuState &currentMenu) {
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKBLUE);
        DrawText("Leaderboard Section", 300, 250, 30, WHITE);
        DrawText("Press SPACE to return to Menu", 250, 300, 20, WHITE);
        EndDrawing();
        if (IsKeyPressed(KEY_SPACE)) {
            currentMenu = MAIN_MENU;
            break;
        }
    }
}

void ShowSettings(MenuState &currentMenu) {
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKBLUE);
        DrawText("Settings Section", 300, 250, 30, WHITE);
        DrawText("Press SPACE to return to Menu", 250, 300, 20, WHITE);
        EndDrawing();
        if (IsKeyPressed(KEY_SPACE)) {
            currentMenu = MAIN_MENU;
            break;
        }
    }
}

void ShowCredits(MenuState &currentMenu) {
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKBLUE);
        DrawText("Credits Section", 300, 250, 30, WHITE);
        DrawText("Press SPACE to return to Menu", 250, 300, 20, WHITE);
        EndDrawing();
        if (IsKeyPressed(KEY_SPACE)) {
            currentMenu = MAIN_MENU;
            break;
        }
    }
}

void QuitGame(MenuState &currentMenu) {
    currentMenu = EXIT;
}
