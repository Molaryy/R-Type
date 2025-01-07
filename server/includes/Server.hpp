/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Server.hpp
*/

#pragma once

#include <dylib.hpp>

#include "Entity.hh"
#include "INetworkServer.hpp"
#include "PacketHandler.hpp"
#include "Registry.hh"

class Server {
public:
    Server(const Server &ref) = delete;
    void operator=(const Server &ref) = delete;
    ~Server() = default;

    static Server &createInstance(std::size_t port, std::size_t maxClients);
    [[nodiscard]] static Server &getInstance();
    [[nodiscard]] Network::PacketHandler &getPacketHandler();
    [[nodiscard]] Network::INetworkServer &getNetwork() const;


    void run();
    void gameLoop();

private:
    Server(std::size_t port, std::size_t maxClients);

    void initPacketHandling();

    dylib networkLoader_;
    std::size_t port_;
    static std::unique_ptr<Server> instance_;

    std::unique_ptr<Network::INetworkServer> networkLib_;
    Network::PacketHandler packetHandler_;
    Registry registry_;
    std::size_t maxClient_;
    std::unordered_map<uint16_t, entity_t> players_;
    bool gameStarted_ = false;
};
