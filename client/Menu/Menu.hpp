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

        struct MenuItem {
            std::string label;
            std::string description;
            std::function<void()> action;
        };

        std::vector<MenuItem> menuItems;
        int selectedOption;
        MenuState currentMenu;

        void playGame(rtype::RayLib &rl);
        void showLeaderboard(rtype::RayLib &rl);
        void showSettings(rtype::RayLib &rl);
        void showCredits(rtype::RayLib &rl);
        void quitGame();

    public:
        Menu(rtype::RayLib &rl);
        void run(rtype::RayLib &rl);
};

#endif // MENU_HPP