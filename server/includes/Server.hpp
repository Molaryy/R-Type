/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Server.hpp
*/

#pragma once

#include <dylib.hpp>
#include <vector>

#include "INetworkServer.hpp"
#include "Lobby.hpp"
#include "PacketHandler.hpp"
#include "Score.hpp"

class Server {
public:
    Server(const Server &ref) = delete;
    void operator=(const Server &ref) = delete;
    ~Server();
    static Server &createInstance(std::size_t port, std::size_t max_lobby, std::size_t max_client, bool debug, bool network_debug, const std::string &path);
    static bool random(double chance);
    [[nodiscard]] static Server &getInstance();
    [[nodiscard]] Network::PacketHandler &getPacketHandler();
    [[nodiscard]] Network::INetworkServer &getNetwork() const;

    void run();

private:
    Server(std::size_t port, std::size_t max_lobby, std::size_t max_client, bool debug, bool network_debug, const std::string &path);

    void gameLoop_();
    void initPacketHandling_();
    void networkReceiver_();

    dylib networkLoader_;
    std::size_t port_;
    std::size_t maxClient_;
    std::size_t nbClient_;
    bool debug_;
    bool network_debug_;
    std::size_t maxLobby_;
    static std::unique_ptr<Server> instance_;

    std::unique_ptr<Network::INetworkServer> networkLib_;
    Network::PacketHandler packetHandler_;
    std::vector<std::unique_ptr<Lobby>> lobbies_;

    bool serverRunning_;

    Score score_;
};
