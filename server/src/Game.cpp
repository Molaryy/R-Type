/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Game.cpp
*/

#include "Game.hpp"
#include "Network.hpp"
#include <iostream>
#include <asio.hpp>
#include <thread>
#include <chrono>

Game::Game() : tick_(0), gameID_(-1) {
    reg_.register_component<Position_t>();
    reg_.register_component<Velocity_t>();
    reg_.register_component<Controllable_t>();
    reg_.register_component<Life_t>();

    auto player = reg_.spawn_entity();

    reg_.add_component(player, Position_t{100.0f, 100.0f});
    reg_.add_component(player, Velocity_t{0.0f, 0.0f});
    reg_.add_component(player, Controllable_t{200.0f});
    reg_.add_component(player, Life_t{100, 100});
}

/**
 * @brief Construct a new Game:: Game object
 * 
 */
void Game::run() {
    std::cout << "Lobby is ready to start with ID: " << gameID_ << std::endl;

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(tickSpeed_));
        auto &positions = reg_.get_components<Position_t>();
        auto &velocities = reg_.get_components<Velocity_t>();

        systems_.MovementSystem(reg_, tickSpeed_);

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
                auto entity = reg_.spawn_entity();
                reg_.add_component(entity, Position_t{50.0f, 50.0f});
                reg_.add_component(entity, Velocity_t{0.0f, 0.0f});
                reg_.add_component(entity, Controllable_t{200.0f});
            } else if (interaction.getQuit() == 1) {
                std::cout << "Player disconnected: " << interaction.getClientID() << std::endl;
            }
        }

        for (const auto &client : clients_) {
            std::string message = serializeGameState();
            client->getSocket().write_some(asio::buffer(message));
        }
        tick_++;
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

std::string Game::serializeGameState() const
{
    std::ostringstream stream;
    auto &positions = reg_.get_components<Position_t>();
    for (std::size_t i = 0; i < positions.size(); ++i) {
        if (positions[i].has_value()) {
            const auto &pos = std::any_cast<const Position_t &>(positions[i]);
            stream << "Entity " << i << " position: (" << pos.x << ", " << pos.y << ")\n";
        }
    }
    return stream.str();
}
