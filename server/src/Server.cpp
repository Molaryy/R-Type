/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Server.cpp
*/

#include "Server.hpp"

#include <iostream>
#include <ranges>

#include "Components.hpp"
#include "RTypeProtocol.hpp"
#include "Systems.hh"
#include "Systems.hpp"

Server::Server(const std::size_t port, const std::size_t maxClients)
    : networkLoader_("./", "asio_server"), port_(port), maxClient_(maxClients) {
    try {
        auto *create_network_lib = networkLoader_.get_function<Network::INetworkServer *()>("create_instance");

        networkLib_.reset(create_network_lib());
    } catch (const dylib::exception &e) {
        throw std::runtime_error("Failed to load shared lib : " + std::string(e.what()));
    }
}

Server &Server::createInstance(const std::size_t port, const std::size_t maxClients) {
    instance_.reset(new Server(port, maxClients));
    return *instance_;
}

Server &Server::getInstance() {
    return *instance_;
}

Network::PacketHandler &Server::getPacketHandler() {
    return packetHandler_;
}

Network::INetworkServer &Server::getNetwork() const {
    return *networkLib_;
}

void Server::run() {
    std::cout << "Starting server..." << std::endl;
    networkLib_->host(port_);

    initPacketHandling();

    registry_.add_system(Systems::networkReceiver);
    registry_.add_system(Systems::log);

    gameLoop();
}


void Server::gameLoop() {
    while (true) {
        registry_.run_systems();
    }
}

void Server::initPacketHandling() {
    packetHandler_.setPacketCallback(
        Protocol::CONNECT,
        [this]([[maybe_unused]] const Network::Packet &packet, const uint16_t client) {
            std::cout << "Client: " << client << " : Ask for connection" << std::endl;
            if (this->gameStarted_) {
                std::cerr << "Game already started !" << std::endl;
                // TODO send error code
                return;
            }

            if (players_.size() >= maxClient_) {
                std::cerr << "Room is full !" << std::endl;
                // TODO send error code
                return;
            }

            const entity_t entity = registry_.spawn_entity();
            registry_.add_component(entity, ClientInputs());
//            registry_.add_component(entity, Position(0, 0));
//            registry_.add_component(entity, Velocity(0, 0));
//            registry_.add_component(entity, HitboxRectangle(PLAYER_SIZE, PLAYER_SIZE));
//            registry_.add_component(entity, HitboxPlan({HitboxPlan::BONUS_ALLY, HitboxPlan::ALLY_ENEMI}));
//            registry_.add_component(entity, Component::ComponentEntityType(PLAYER));
//            registry_.add_component(entity, ClockComponent(PLAYER_SHOOT_RATE));
//            registry_.add_component(entity, Health(PLAYER_HEALTH));
            players_.emplace(client, entity);


            Network::Packet response(Protocol::AcceptConnectionPacket(static_cast<std::size_t>(entity)), Protocol::ACCEPT_CONNECTION);
            networkLib_->send(client, response.serialize());
        });

    packetHandler_.setPacketCallback(
        Protocol::CLIENT_ASK_START_GAME,
        [this]([[maybe_unused]] const Network::Packet &packet, const uint16_t client) {
            std::cout << "Client: " << client << " : Try starting game" << std::endl;

            if (this->gameStarted_) {
                std::cout << "Game already started !" << std::endl;
                // TODO send error code
                return;
            }

            Network::Packet response(Protocol::EmptyPacket(), Protocol::START_GAME);
            networkLib_->sendAll(response.serialize());

            for (const auto &pla : players_ | std::views::values) {
                Network::Packet new_player_packet(
                    Protocol::SpawnEntityPacket(
                        static_cast<std::size_t>(pla),
                        Protocol::PLAYER,
                        Protocol::Vector2i(0, 0),
                        Protocol::Vector2i(0, 0)),
                    Protocol::SPAWN
                );
                networkLib_->sendAll(new_player_packet.serialize());
            }
            gameStarted_ = true;
        });

    packetHandler_.setPacketCallback(
        Protocol::INPUT_KEYS,
        [this](const Network::Packet &packet, const uint16_t client) {
            std::cout << "Client: " << client << " : Sended inputs: " << std::endl;

            auto [key_pressed] = packet.getPayload<Protocol::InputsKeysPacket>();

            SparseArray<ClientInputs> &inputs = registry_.get_components<ClientInputs>();


            if (!players_.contains(client) || !inputs[players_.at(client)].has_value()) {
                std::cerr << "Client " << client << " isn't connected" << std::endl;
                return;
            }
            inputs[players_.at(client)].value().setInputs(key_pressed);
            for (int i = 0; i < Protocol::NB_INPUTS_KEYS; i++)
                std::cout << std::boolalpha << key_pressed[i] << std::endl;
        });
}

std::unique_ptr<Server> Server::instance_ = nullptr;
