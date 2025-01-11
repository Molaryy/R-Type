/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Lobby.cpp
*/

#include "Lobby.hpp"

#include <mutex>
#include <Systems.hh>

#include "Components.hh"
#include "Components.hpp"
#include "Packet.hpp"
#include "Server.hpp"

Lobby::Lobby(const std::size_t maxClient)
    : networkLib_(Server::getInstance().getNetwork()),
      maxClient_(maxClient),
      state_(Protocol::OPEN) {
    static std::size_t next_lobby = 0;
    lobbyId_ = next_lobby++;
    thread_ = std::thread(&Lobby::run, this);
}

Lobby::~Lobby() = default;

std::size_t Lobby::getId() const {
    return lobbyId_;
}

Protocol::LobbyState Lobby::getState() const {
    if (players_.size() == maxClient_)
        return Protocol::FULL;
    return state_;
}

bool Lobby::containPlayer(const uint16_t network_id) const {
    return players_.contains(network_id);
}

std::size_t Lobby::getNbPLayers() const {
    return players_.size();
}

void Lobby::addPlayer(const uint16_t client) {
    {
        std::unique_lock lock(networkMutex_);

        networkTasks_.push([client, this] {
            if (players_.contains(client)) {
                std::cerr << "Player " << client << " is already connected" << std::endl;
                // TODO send error code
                return;
            }
            if (state_ == Protocol::CLOSE) {
                std::cerr << "Lobby is close" << std::endl;
                // TODO send error code
                return;
            }
            if (state_ == Protocol::IN_GAME) {
                std::cerr << "Lobby is already in game" << std::endl;
                // TODO send error code
                return;
            }
            if (players_.size() >= maxClient_) {
                std::cerr << "Lobby id full" << std::endl;
                // TODO send error code
                return;
            }

            const entity_t entity = registry_.spawn_entity();
            registry_.add_component(entity, ClientInputs());
            registry_.add_component(entity, Position(0, 0));
            registry_.add_component(entity, Velocity(0, 0));
            registry_.add_component(entity, Delay(PLAYER_SHOOT_RATE, 0));
            registry_.add_component(entity, Life(PLAYER_HEALTH, PLAYER_HEALTH));
            players_.emplace(client, entity);

            Network::Packet response(Protocol::AcceptLobbyJoinPacket(lobbyId_, entity), Protocol::ACCEPT_CONNECTION);
            networkLib_.send(client, response.serialize());
        });
    }
}

void Lobby::startGame() {
    {
        std::unique_lock lock(networkMutex_);

        networkTasks_.push([this] {
            if (state_ == Protocol::IN_GAME) {
                std::cout << "Lobby already started" << std::endl;
                // TODO send error code
                return;
            }
            if (state_ == Protocol::CLOSE) {
                std::cout << "Lobby is closed" << std::endl;
                // TODO send error code
                return;
            }

            Network::Packet response(Protocol::EmptyPacket(), Protocol::START_GAME);
            networkLib_.sendAll(response.serialize());

            for (const auto &pla : players_ | std::views::values) {
                Network::Packet new_player_packet(
                    Protocol::SpawnEntityPacket(
                        static_cast<std::size_t>(pla),
                        Protocol::PLAYER,
                        Protocol::Vector2i(0, 0),
                        Protocol::Vector2i(0, 0)),
                    Protocol::SPAWN
                );
                networkLib_.sendAll(new_player_packet.serialize());
            }
            state_ = Protocol::IN_GAME;
        });
    }
}

void Lobby::setInputKeys(bool key_pressed[Protocol::NB_INPUTS_KEYS], const uint16_t client) {
    {
        std::unique_lock lock(networkMutex_);

        networkTasks_.push([key_pressed, client, this] {
            SparseArray<ClientInputs> &inputs = registry_.get_components<ClientInputs>();

            if (!players_.contains(client) || !inputs[players_.at(client)].has_value()) {
                std::cerr << "Client " << client << " isn't connected" << std::endl;
                return;
            }
            inputs[players_.at(client)].value().setInputs(key_pressed);
        });
    }
}

void Lobby::run() {
    registry_.add_system([this]([[maybe_unused]] const Registry &r) {
        {
            std::unique_lock lock(networkMutex_);

            while (!networkTasks_.empty()) {
                std::function<void()> function = networkTasks_.front();
                function();
                networkTasks_.pop();
            }
        }
    });
    registry_.add_system(Systems::position_velocity);
    registry_.add_system(Systems::log);
    registry_.add_system([](Registry &r) {
        Systems::limit_framerate(r, SERVER_TPS);
    });

    while (true) {
        registry_.run_systems();
    }
}
