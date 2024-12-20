/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Server.cpp
*/

#include "Server.hpp"

/**
 * @brief Construct a new Server:: Server object
 * 
 */
void Server::run()
{
    std::cout << "Starting server..." << std::endl;
    gameThread_ = std::thread(&Server::gameLoop, this);
    checkClientThread_ = std::thread(&Server::monitorClients, this);
    network_.run();
    if (gameThread_.joinable())
        gameThread_.join();
    if (checkClientThread_.joinable())
        checkClientThread_.join();
}


void Server::gameLoop()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(16));

        for (const auto &game : network_.getGames()) {
            std::string gameState = game->serializeGameState();
            // for (const auto &client : network_.getClients())
            // {
            //     network_.writeToClient(client, gameState);
            // }
        }
    }
}

void Server::monitorClients()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
