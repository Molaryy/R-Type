/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Lobby.hpp
*/

#pragma once

#include <cstddef>

#include "INetworkServer.hpp"
#include "Registry.hh"
#include "RTypeProtocol.hpp"

class Lobby {
public:
    explicit Lobby(std::size_t maxClient);
    ~Lobby();

    [[nodiscard]] std::size_t getId() const;
    [[nodiscard]] Protocol::LobbyState getState() const;
    [[nodiscard]] bool containPlayer(uint16_t network_id) const;
    [[nodiscard]] std::size_t getNbPLayers() const;

    void addPlayer(uint16_t client);
    void startGame();
    void setInputKeys(bool key_pressed[Protocol::NB_INPUTS_KEYS], uint16_t client);

    void run();

private:
    Network::INetworkServer &networkLib_;

    std::size_t lobbyId_;
    Registry registry_;
    std::size_t maxClient_;
    std::unordered_map<uint16_t, entity_t> players_;
    Protocol::LobbyState state_;
};
