/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Lobby
*/

#pragma once

#include <functional>

#include "Client.hpp"
#include "RTypeProtocol.hpp"
#include "Registry.hh"

class Lobby
{
public:
    Lobby() = default;
    ~Lobby() = default;
    static bool timeOut(const std::chrono::system_clock::time_point time_out_clock, Network::INetworkClient &network);
    void leaveLobby();
    std::vector<Protocol::LobbyDataPacket> getLobbyList(Network::INetworkClient &network, Network::PacketHandler &packet_handler);
    Protocol::LobbyDataPacket getLobbyData(const std::size_t lobby_id);
    void startLobby(Registry &r);
    void lobbyPage(Registry &r, const std::size_t lobby_id);
    void joinLobby(Registry &r, const std::function<void()> &send_packet);
    void lobbyCallback([[maybe_unused]] Registry &r);
private:
};
