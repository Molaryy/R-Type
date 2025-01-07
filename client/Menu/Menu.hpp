#ifndef MENU_HPP
    #define MENU_HPP

#include "../Gameplay/Game.hpp"
#include "../raylib/raylib.hh"
#include <functional>
#include <vector>
#include <string>

struct PlayerScore {
    std::string name;
    int score;
};

std::vector<PlayerScore> loadScores(const std::string &filename);

class Menu {
    private:

        enum MenuState {
            MAIN_MENU,
            PLAY_GAME,
            LEADERBOARD,
            SETTINGS,
            CREDITS,
            EXIT
        };

        struct Lobby {
            std::string name;
            std::vector<std::string> players;
            bool gameStarted;

            Lobby(const std::string& lobbyName) : name(lobbyName), gameStarted(false) {}
        };

        struct MenuItem {
            std::string label;
            std::string description;
            std::function<void()> action;
        };

        std::vector<MenuItem> menuItems;
        int selectedOption;
        MenuState currentMenu;
        std::vector<Lobby> lobbies;

        void playGame(rtype::RayLib &rl);
        void showLeaderboard(rtype::RayLib &rl);
        void drawSettingsOptions(rtype::RayLib &rl, int selectedOption, int currentFPSIndex,
        bool musicEnabled, bool soundEffectsEnabled, int currentResolutionIndex, bool colorBlindMode);
        void showSettings(rtype::RayLib &rl);
        void showCredits(rtype::RayLib &rl);
        void quitGame();
        void showLobbies(rtype::RayLib &rl);
        void joinLobby(rtype::RayLib &rl, int lobbyIndex);

    public:
        Menu(rtype::RayLib &rl);
        void run(rtype::RayLib &rl);
};

#endif // MENU_HPP