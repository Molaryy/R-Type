/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Lobby.hpp
*/

#pragma once

#include <cstddef>
#include <mutex>
#include <queue>
#include <thread>

#include "INetworkServer.hpp"
#include "Registry.hh"
#include "RTypeProtocol.hpp"

class Lobby {
public:
    Lobby(std::size_t maxClient, bool debug);
    ~Lobby();

    [[nodiscard]] std::size_t getId() const;
    [[nodiscard]] Protocol::LobbyState getState() const;
    [[nodiscard]] bool containPlayer(uint16_t network_id) const;
    [[nodiscard]] std::size_t getNbPLayers() const;

    void addPlayer(uint16_t client);
    void leavePlayer(uint16_t client);
    void startGame();
    void setInputKeys(std::vector<Protocol::InputKey> key_pressed, uint16_t client);


private:
    void run_();
    static void executeNetworkSystem_(const Registry &r, Lobby &lobby);

    Network::INetworkServer &networkLib_;

    std::size_t lobbyId_;
    Registry registry_;
    std::size_t maxClient_;
    bool debug_;
    std::unordered_map<uint16_t, entity_t> players_;
    Protocol::LobbyState state_;

    std::queue<std::function<void()>> networkTasks_;

    std::thread thread_;
    std::mutex networkMutex_;
};
