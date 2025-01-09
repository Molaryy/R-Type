/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** control system
*/

#pragma once

#include <cstddef>

#include "Client.hpp"
#include "Entity.hpp"
#include "INetworkClient.hpp"
#include "PacketHandler.hpp"
#include "RTypeProtocol.hpp"
#include "Registry.hh"

namespace System {
    inline void networkReceiver([[maybe_unused]] Registry &r) {
        auto &core = R_Type::Client::getInstance();
        auto &network = core.getNetworkLib();
        auto &packet_handler = core.getPacketHandler();

        std::vector<uint8_t> oldest_packet = network.getOldestPacket();

        while (!oldest_packet.empty()) {
            ::Network::Packet deserialized_packet(oldest_packet);
            packet_handler(deserialized_packet);

            if (deserialized_packet.getCommandId() == R_Type::START_GAME) {
                break;
            }
            oldest_packet = network.getOldestPacket();
        }
    }
}
