/*
** EPITECH PROJECT, 2025
** B-YEP-400_R-Type
** File description:
** Systems.hpp
*/

#pragma once

namespace Systems {
    void networkReceiver([[maybe_unused]] Registry &r) {
        Network::INetworkServer &network_lib = Server::getInstance().getNetwork();

        for (auto all_oldest_packet = network_lib.getAllOldestPacket(); !all_oldest_packet.empty(); all_oldest_packet = network_lib.getAllOldestPacket()) {
            for (const auto &[cli, payload] : all_oldest_packet) {
                Network::Packet packet(payload);
                Server::getInstance().getPacketHandler()(packet, cli);
            }
        }
    }
}
