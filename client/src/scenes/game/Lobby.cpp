/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Lobby
*/

#include "Lobby.hpp"

#include "Scenes.hpp"
#include "RTypeProtocol.hpp"
#include "Components.hpp"
#include "Components.hh"
#include "Client.hpp"

std::vector<Protocol::LobbyDataPacket> reloadLobbyList(Network::INetworkClient &network, Network::PacketHandler &packet_handler)
{
    bool get_lobby_list = false;
    std::size_t nb_lobby;
    std::vector<Protocol::LobbyDataPacket> lobby_data;

    packet_handler.setPacketCallback(Protocol::LOBBY_LIST,
        [&nb_lobby, &get_lobby_list, &network](const Network::Packet &packet)
    {
        get_lobby_list = true;

        Protocol::LobbyListPacket list = packet.getPayload<Protocol::LobbyListPacket>();
        nb_lobby = list.number_of_lobbys;
        std::cout << "Actually " << nb_lobby << "lobby created on server" << std::endl;

        for (std::size_t i = 0; i < nb_lobby; ++i)
        {
            Network::Packet packetSended(Protocol::AskLobbyDataPacket(), Protocol::ASK_LOBBY_DATA);
            network.send(packetSended.serialize());
        }
    });

    packet_handler.setPacketCallback(Protocol::LOBBY_DATA,
        [&lobby_data](const Network::Packet &packet)
    {
        Protocol::LobbyDataPacket data = packet.getPayload<Protocol::LobbyDataPacket>();
        lobby_data.push_back(data);
    });

    Network::Packet packet(Protocol::EmptyPacket(), Protocol::ASK_LOBBY_LIST);
    network.send(packet.serialize());

    const std::chrono::system_clock::time_point time_out_clock = std::chrono::system_clock::now();

    while (!get_lobby_list)
    {
        const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        if (const std::chrono::duration<double> elapsed_seconds = now - time_out_clock; elapsed_seconds.count() > 3.0)
            throw std::runtime_error("Can't get lobby list: time out");
        std::vector<uint8_t> oldest_packet = network.getOldestPacket();
        if (oldest_packet.empty())
            continue;
        Network::Packet deserialized_packet(oldest_packet);
        packet_handler(deserialized_packet);
    }
    while (lobby_data.size() < nb_lobby)
    {
        const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        if (const std::chrono::duration<double> elapsed_seconds = now - time_out_clock; elapsed_seconds.count() > 3.0)
            throw std::runtime_error("Can't get lobby data: time out");
        std::vector<uint8_t> oldest_packet = network.getOldestPacket();
        if (oldest_packet.empty())
            continue;
        Network::Packet deserialized_packet(oldest_packet);
        packet_handler(deserialized_packet);
    }
    return lobby_data;
}

void lobbyCallback(Registry &r)
{
    static bool needed_reload = true;
    Network::INetworkClient &network = Client::getInstance().getNetworkLib();
    Network::PacketHandler &packet_handler = Client::getInstance().getPacketHandler();
    std::vector<Protocol::LobbyDataPacket> lobby_list{};
    if (needed_reload)
    {
        lobby_list = reloadLobbyList(network, packet_handler);
        needed_reload = false;
        for (std::size_t i = 0; i < lobby_list.size(); ++i)
        {
            std::cout << "Lobby " << i << " : " << lobby_list[i].lobby_id << " - " << lobby_list[i].nb_players << " players" << std::endl;
        }
    }
}
