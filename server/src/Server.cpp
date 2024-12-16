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

        // test without ecs
        auto game = network_.getGame(1);
        if (!game) {
            std::cerr << "No game found with ID 1!" << std::endl;
            continue;
        }

        auto entities = game->getEntities();

        // for (const auto& client : network_.getClients())
        // {
            // for (const auto& entity : entities)
            // {
                // std::string message = "Entity " + std::to_string(entity.id) +
                //     " at position (" + std::to_string(entity.x) + ", " + std::to_string(entity.y) + ")";
                // network_.writeToClient(client, message);
            // }
        // }
    }
}

void Server::monitorClients()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
