/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** LeaderBoard
*/

#include "Score.hpp"
#include "Client.hpp"
#include "Components.hpp"
#include "RTypeProtocol.hpp"
#include "Scenes.hpp"

Protocol::ScoreboardPacket getScoreboard()
{
    Protocol::ScoreboardPacket scoreboard{};
    bool received_scoreboard = false;
    Network::INetworkClient &network = Client::getInstance().getNetworkLib();
    Network::PacketHandler &packet_handler = Client::getInstance().getPacketHandler();

    packet_handler.setPacketCallback(Protocol::SCOREBOARD, [&scoreboard, &received_scoreboard](const Network::Packet &packet)
    {
        received_scoreboard = true;
        scoreboard = packet.getPayload<Protocol::ScoreboardPacket>();
    });

    Network::Packet packetSended(Protocol::EmptyPacket(), Protocol::ASK_SCOREBOARD);
    network.send(packetSended.serialize());

    const std::chrono::system_clock::time_point time_out_clock = std::chrono::system_clock::now();

    while (!received_scoreboard)
    {
        const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        if (const std::chrono::duration<double> elapsed_seconds = now - time_out_clock; elapsed_seconds.count() > 3.0)
            throw std::runtime_error("Can't get scoreboard: time out");
        std::vector<uint8_t> oldest_packet = network.getOldestPacket();
        if (oldest_packet.empty())
            continue;
        Network::Packet deserialized_packet(oldest_packet);
        packet_handler(deserialized_packet);
    }

    return scoreboard;
}

Protocol::ScoreboardIdPacket getIdScoreboard(std::string name)
{
    Protocol::ScoreboardIdPacket scoreboard{};
    bool received_scoreboard = false;
    Network::INetworkClient &network = Client::getInstance().getNetworkLib();
    Network::PacketHandler &packet_handler = Client::getInstance().getPacketHandler();

    packet_handler.setPacketCallback(Protocol::SCOREBOARD_ID, [&scoreboard, &received_scoreboard](const Network::Packet &packet)
    {
        received_scoreboard = true;
        scoreboard = packet.getPayload<Protocol::ScoreboardIdPacket>();
    });

    std::strncpy(scoreboard.name, name.c_str(), NAME_SIZE - 1);
    Network::Packet packetSended(scoreboard, Protocol::ASK_SCOREBOARD_ID);
    network.send(packetSended.serialize());

    const std::chrono::system_clock::time_point time_out_clock = std::chrono::system_clock::now();

    while (!received_scoreboard)
    {
        const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        if (const std::chrono::duration<double> elapsed_seconds = now - time_out_clock; elapsed_seconds.count() > 3.0)
            throw std::runtime_error("Can't get scoreboard: time out");
        std::vector<uint8_t> oldest_packet = network.getOldestPacket();
        if (oldest_packet.empty())
            continue;
        Network::Packet deserialized_packet(oldest_packet);
        packet_handler(deserialized_packet);
    }

    return scoreboard;
}
