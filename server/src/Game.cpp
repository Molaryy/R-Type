/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Game.cpp
*/

#include "Game.hpp"

namespace server {
    Game::Game() {
        _tick = 0;
    }

    void Game::run() {
        std::cout << "Game started" << std::endl;
        srand(time(NULL));
        int clientTd = 0;
        bool startGame = false;
        std::vector<Interaction> interaction_client;

        while (true) {
            _mutex.lock();
            if (_interaction_client.size() > 0) {
                interaction_client = _interaction_client;
                _interaction_client.clear();
            }
            _mutex.unlock();
            if (_interaction_client.size() > 0) {
                for (auto interaction : interaction_client) {
                    if (interaction.getConnect() == 1) {
                        _mutex.lock();
                        std::cout << "New Player with ID: " << interaction.getClientID() << std::endl;
                        startGame = true;
                        interaction.setMovement(0);
                        _mutex.unlock();
                        continue;
                    }
                    if (interaction.getQuit() == 1) {
                        std::cout << "Player with ID : " << interaction.getClientID() << " quit" << std::endl;
                        interaction.setMovement(0);
                    }
                }
            }
            _functions.clear();
            _tick++;
            interaction_client.clear();
            std::this_thread::sleep_for(std::chrono::milliseconds(_tickSpeed));
        }
    }
}
