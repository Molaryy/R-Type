/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Lobby.cpp
*/

#include "Lobby.hpp"

#include <mutex>

#include "Components.hpp"
#include "Packet.hpp"
#include "Server.hpp"
#include "Systems.hh"
#include "Systems.hpp"
#include "entities/Player.hpp"

Lobby::Lobby(const std::size_t maxClient, const bool debug)
    : networkLib_(Server::getInstance().getNetwork()),
      maxClient_(maxClient),
      debug_(debug),
      state_(Protocol::OPEN) {
    static std::size_t next_lobby = 0;
    lobbyId_ = next_lobby++;
    thread_ = std::thread(&Lobby::run_, this);
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

        networkTasks_.emplace([client, this] {
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

            const entity_t entity = Player::create(registry_);
            players_.emplace(client, entity);

            Network::Packet response(Protocol::AcceptLobbyJoinPacket(lobbyId_, entity), Protocol::ACCEPT_LOBBY_JOIN);
            networkLib_.send(client, response.serialize());
        });
    }
}

void Lobby::startGame() {
    {
        std::unique_lock lock(networkMutex_);

        networkTasks_.emplace([this] {
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
                        pla,
                        Protocol::PLAYER,
                        Protocol::Vector2f(0, 0),
                        Protocol::Vector2f(0, 0)),
                    Protocol::SPAWN
                );
                networkLib_.sendAll(new_player_packet.serialize());
            }
            registry_.clear_systems();

            registry_.add_system([this]([[maybe_unused]] const Registry &r) {
                executeNetworkSystem_(r, *this);
            });
            registry_.add_system(Systems::handleClientInputs);
            registry_.add_system(Systems::position_velocity);
            registry_.add_system(Systems::levelHandler);
            registry_.add_system(Systems::generic_collide);
            registry_.add_system(Systems::sendGameState);
            if (debug_)
                registry_.add_system(Systems::log);
            registry_.add_system([](Registry &r) {
                Systems::limit_framerate(r, SERVER_TPS);
            });

            state_ = Protocol::IN_GAME;
        });
    }
}

void Lobby::setInputKeys(std::vector<Protocol::InputKey> key_pressed, const uint16_t client) {
    {
        std::unique_lock lock(networkMutex_);

        networkTasks_.emplace([key_pressed, client, this] {
            SparseArray<ClientInputs> &inputs = registry_.get_components<ClientInputs>();

            if (!players_.contains(client) || !inputs[players_.at(client)].has_value()) {
                std::cerr << "Client " << client << " isn't connected" << std::endl;
                return;
            }
            inputs[players_.at(client)].value().setInputs(key_pressed);
        });
    }
}

void Lobby::executeNetworkSystem_([[maybe_unused]] const Registry &r, Lobby &lobby) {
    {
        std::unique_lock lock(lobby.networkMutex_);

        while (!lobby.networkTasks_.empty()) {
            std::function<void()> function = lobby.networkTasks_.front();
            function();
            lobby.networkTasks_.pop();
        }
    }
}

void Lobby::run_() {
    registry_.add_system([this]([[maybe_unused]] const Registry &r) {
        executeNetworkSystem_(r, *this);
    });
    if (debug_)
        registry_.add_system(Systems::log);
    registry_.add_system([](Registry &r) {
        Systems::limit_framerate(r, SERVER_TPS);
    });

    while (true) {
        registry_.run_systems();
    }
}
