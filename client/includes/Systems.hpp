/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** control system
*/

#pragma once

#include "Client.hpp"
#include "INetworkClient.hpp"
#include "PacketHandler.hpp"
#include "Registry.hh"

namespace Systems {
    inline void networkReceiver([[maybe_unused]] Registry &r) {
        auto &core = Client::getInstance();
        auto &network = core.getNetworkLib();
        auto &packet_handler = core.getPacketHandler();

        for (std::vector<uint8_t> oldest_packet = network.getOldestPacket(); !oldest_packet.empty(); oldest_packet = network.getOldestPacket()) {
            Network::Packet deserialized_packet(oldest_packet);
            packet_handler(deserialized_packet);
        }
    }
}
