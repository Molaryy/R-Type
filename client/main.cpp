#include "raylib/raylib.hh"
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
    Camera camera = { 0 };
    camera.position = (Vector3){ 0.0f, 10.0f, 20.0f }; // Pos caméra
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };     // Point que regarde la caméra
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };         // haut
    camera.fovy = 45.0f;                              // POV
    camera.projection = CAMERA_PERSPECTIVE;           // Perspective MODE

    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };
    float cubeRotation = 0.0f;

    while (!rl.windowShouldClose()) {
        if (rl.isKeyPressed(KEY_SPACE)) {
            currentMenu = MAIN_MENU;
            break;
        }
        cubeRotation += 1.0f;

        rl.beginDrawing();
        rl.clearBackground(BLACK);
        rl.beginMode3D(camera);
        rl.drawCube(cubePosition, 2.0f, 2.0f, 2.0f, DARKBLUE);
        rl.drawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, DARKGRAY);
        rl.drawGrid(100, 1.0f);
        rl.endMode3D();
        rl.drawText("Press SPACE to return to Menu", 10, 10, 20, WHITE);
        rl.endDrawing();
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
