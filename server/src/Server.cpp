/*
** EPITECH PROJECT, 2024
** B-CPP-500-PAR-5-2-bsrtype-arthaud.poupard
** File description:
** Server.cpp
*/

// #include "Server.hpp"

// int server::Server::run() {
//     _network.run();
//     return 0;
// }

#include "Server.hpp"

namespace server {

    int Server::run() {
        try {
            std::cout << "Starting server..." << std::endl;

            _gameThread = std::thread(&Server::gameLoop, this);
            _checkClientThread = std::thread(&Server::monitorClients, this);

            _network.run();

            if (_gameThread.joinable())
                _gameThread.join();
            if (_checkClientThread.joinable())
                _checkClientThread.join();

            return 0;
        } catch (const std::exception& e) {
            std::cerr << "Server error: " << e.what() << std::endl;
            return 84;
        }
    }

    void Server::gameLoop() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    }

    void Server::monitorClients() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}
