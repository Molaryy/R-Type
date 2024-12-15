#ifndef MENU_HPP
    #define MENU_HPP

#include "../Gameplay/Game.hpp"
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

        struct MenuItem {
            std::string label;
            std::string description;
            std::function<void()> action;
        };

        std::vector<MenuItem> menuItems;
        int selectedOption;
        MenuState currentMenu;

        void playGame();
        void showLeaderboard();
        void showSettings();
        void showCredits();
        void quitGame();

    public:
        Menu();
        void run();
};

#endif // MENU_HPP