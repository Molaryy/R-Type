/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Game.cpp
*/

#include "Game.hpp"
#include <iostream>
#include <thread>
#include <chrono>

Game::Game() : tick_(0), gameID_(-1), availableID_(1) {
    entities_ = {
        {1, "player", 100.0f, 200.0f, 0.0f, 0.0f},
        {2, "projectile", 300.0f, 400.0f, -1.0f, 0.0f}
    };
}

/**
 * @brief Construct a new Game:: Game object
 * 
 */
// void Game::run()
// {
//     std::cout << "Lobby is ready to start with ID: " << gameID_ << std::endl;
//     while (true)
//     {
//         std::this_thread::sleep_for(std::chrono::milliseconds(tickSpeed_));
//         std::vector<Interaction> localInteractions;
//         {
//             std::lock_guard<std::mutex> lock(_mutex);
//             localInteractions = interactionClient_;
//             interactionClient_.clear();
//         }

//         for (const auto &interaction : localInteractions)
//         {
//             if (interaction.getConnect() == 1) {
//                 std::cout << "Player connected: " << interaction.getClientID() << std::endl;
//             } else if (interaction.getQuit() == 1) {
//                 std::cout << "Player disconnected: " << interaction.getClientID() << std::endl;
//             } else if (interaction.getMovement() != -1) {
//                 std::cout << "Player " << interaction.getClientID()
//                     << " moved to position " << interaction.getMovement() << std::endl;
//             }
//         }
//         tick_++;
//         if (tick_ % 100 == 0)
//             std::cout << "Game tick: " << tick_ << std::endl;
//         // Update game next
//     }
// }

void Game::run() {
    std::cout << "Lobby is ready to start with ID: " << gameID_ << std::endl;

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(tickSpeed_));
        for (auto &entity : entities_) {
            entity.x += entity.velocityX;
            entity.y += entity.velocityY;
            // if (entity.type == "player")
            //     std::cout << "Player " << entity.id << " is at position (" << entity.x << ", " << entity.y << ")" << std::endl;
            // else if (entity.type == "projectile")
            //     std::cout << "Projectile " << entity.id << " is at position (" << entity.x << ", " << entity.y << ")" << std::endl;
        }

        std::vector<Interaction> localInteractions;
        {
            std::lock_guard<std::mutex> lock(_mutex);
            localInteractions = interactionClient_;
            interactionClient_.clear();
        }

        for (const auto &interaction : localInteractions)
        {
            if (interaction.getConnect() == 1) {
                std::cout << "Player connected: " << interaction.getClientID() << std::endl;
            } else if (interaction.getQuit() == 1) {
                std::cout << "Player disconnected: " << interaction.getClientID() << std::endl;
            } else if (interaction.getMovement() != -1) {
                std::cout << "Player " << interaction.getClientID()
                    << " moved to position " << interaction.getMovement() << std::endl;
            }
        }

        tick_++;
        if (tick_ % 100 == 0)
            std::cout << "Game tick: " << tick_ << std::endl;
    }
}

/**
 * @brief Get the Game ID object
 * 
 * @return int 
 */
void Game::addInteraction(const Interaction &interaction)
{
    std::lock_guard<std::mutex> lock(_mutex);
    interactionClient_.push_back(interaction);
}
