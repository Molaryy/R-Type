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

Server::Server(const std::size_t port, const std::size_t maxClients)
    : networkLoader_("./", "asio_server"),
      port_(port),
      maxClient_(maxClients),
      serverRunning_(true) {
    try {
        auto *create_network_lib = networkLoader_.get_function<Network::INetworkServer *()>("create_instance");

        networkLib_.reset(create_network_lib());
    } catch (const dylib::exception &e) {
        throw std::runtime_error("Failed to load shared lib : " + std::string(e.what()));
    }
}

Server::~Server() {
    networkLib_->close();
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

    initPacketHandling_();

    networkLib_->host(port_);

    std::cout << "Server is hosting on port " << port_ << std::endl;

    gameLoop_();
}


void Server::networkReceiver_() {
    std::vector<std::pair<uint16_t, std::vector<uint8_t>>> all_oldest_packet = networkLib_->getAllOldestPacket();
    while (!all_oldest_packet.empty()) {
        for (const auto &[cli, payload] : all_oldest_packet) {
            Network::Packet packet(payload);
            packetHandler_(packet, cli);
        }
        all_oldest_packet = networkLib_->getAllOldestPacket();
    }
}

void Server::gameLoop_() {
    while (serverRunning_) {
        networkReceiver_();
    }
}

void Server::initPacketHandling_() {
    packetHandler_.setPacketCallback(
        Protocol::ASK_LOBBY_LIST,
        [this]([[maybe_unused]] const Network::Packet &packet, const uint16_t client) {
            std::cout << "Client: " << client << " : Ask for number of lobbies : " << lobbies_.size() << std::endl;

            Network::Packet lobbyList(Protocol::LobbyListPacket(lobbies_.size()), Protocol::CommandIdServer::LOBBY_LIST);
            networkLib_->send(client, lobbyList.serialize());
        });
    packetHandler_.setPacketCallback(
        Protocol::ASK_LOBBY_DATA,
        [this](const Network::Packet &packet, const uint16_t client) {
            const auto [lobby_id] = packet.getPayload<Protocol::AskLobbyDataPacket>();

            std::cout << "Client: " << client << " : Ask data of lobby " << lobby_id << std::endl;

            Protocol::LobbyDataPacket lobbyDataPacket(lobby_id, Protocol::LobbyState::CLOSE, 0);

            const auto &it = std::ranges::find_if(lobbies_, [lobby_id](const std::unique_ptr<Lobby> &lobby) {
                return lobby->getId() == lobby_id;
            });
            if (it != lobbies_.end()) {
                const std::unique_ptr<Lobby> &lobby = *it;
                lobbyDataPacket.lobby_state = lobby->getState();
                lobbyDataPacket.nb_players = lobby->getNbPLayers();
            }

            Network::Packet lobbyData(lobbyDataPacket, Protocol::CommandIdServer::LOBBY_DATA);
            networkLib_->send(client, lobbyData.serialize());
        });
    packetHandler_.setPacketCallback(
        Protocol::JOIN_LOBBY_BY_ID,
        [this](const Network::Packet &packet, const uint16_t client) {
            std::cout << "Client: " << client << " : Join lobby" << std::endl;

            const auto [lobby_id] = packet.getPayload<Protocol::JoinLobbyPacket>();

            const auto it = std::ranges::find_if(lobbies_, [lobby_id](const std::unique_ptr<Lobby> &lobby) {
                return lobby->getId() == lobby_id;
            });
            if (it == lobbies_.end()) {
                std::cerr << "Lobby" << lobby_id << " is closed" << std::endl;
                return;
            }
            it->get()->addPlayer(client);
        });
    packetHandler_.setPacketCallback(
        Protocol::JOIN_NEW_LOBBY,
        [this]([[maybe_unused]] const Network::Packet &packet, const uint16_t client) {
            std::cout << "Client: " << client << " : Join a newly created lobby" << std::endl;

            lobbies_.push_back(std::make_unique<Lobby>(maxClient_));
            lobbies_.back().get()->addPlayer(client);
        });
    packetHandler_.setPacketCallback(
        Protocol::JOIN_RANDOM_LOBBY,
        [this]([[maybe_unused]] const Network::Packet &packet, const uint16_t client) {
            std::cout << "Client: " << client << " : Join a random lobby" << std::endl;

            for (const std::unique_ptr<Lobby> &lobby : lobbies_) {
                if (lobby->getState() != Protocol::OPEN)
                    continue;
                lobby->addPlayer(client);
                return;
            }
            lobbies_.push_back(std::make_unique<Lobby>(maxClient_));
            lobbies_.back().get()->addPlayer(client);
        });
    packetHandler_.setPacketCallback(
        Protocol::CONNECT,
        [this]([[maybe_unused]] const Network::Packet &packet, const uint16_t client) {
            std::cout << "Client: " << client << " : Ask for connection" << std::endl;

            Network::Packet response(Protocol::EmptyPacket(), Protocol::ACCEPT_CONNECTION);
            networkLib_->send(client, response.serialize());
        });
    packetHandler_.setPacketCallback(
        Protocol::ASK_START_GAME,
        [this]([[maybe_unused]] const Network::Packet &packet, const uint16_t client) {
            std::cout << "Client: " << client << " : Try starting game" << std::endl;

            const auto it = std::ranges::find_if(lobbies_, [client](const std::unique_ptr<Lobby> &lobby) {
                return lobby->containPlayer(client);
            });
            if (it == lobbies_.end()) {
                std::cerr << "Client " << client << " isn't connected in any lobby" << std::endl;
                return;
            }
            it->get()->startGame();
        });
    packetHandler_.setPacketCallback(
        Protocol::INPUT_KEYS,
        [this](const Network::Packet &packet, const uint16_t client) {
            std::cout << "Client: " << client << " : Sended inputs: " << std::endl;

            const auto it = std::ranges::find_if(lobbies_, [client](const std::unique_ptr<Lobby> &lobby) {
                return lobby->containPlayer(client);
            });
            if (it == lobbies_.end()) {
                std::cerr << "Client " << client << " isn't connected in any lobby" << std::endl;
                return;
            }
            auto [key_pressed] = packet.getPayload<Protocol::InputsKeysPacket>();
            it->get()->setInputKeys(key_pressed, client);
        });
}

std::unique_ptr<Server> Server::instance_ = nullptr;
