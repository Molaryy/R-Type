/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** PacketHandler.cpp
*/

#include "PacketHandler.hpp"

#include <iostream>
#include <ostream>

namespace Network {
    PacketHandler::PacketHandler() = default;

    PacketHandler::~PacketHandler() = default;

    void PacketHandler::setPacketCallback(const uint16_t command_id, const std::function<void(Packet &)> &callback) {
        _packet_callbacks[command_id] = [callback](Packet &packet, [[maybe_unused]] uint16_t client) { callback(packet); };
    }

    void PacketHandler::setPacketCallback(const uint16_t command_id, const std::function<void(Packet &, uint16_t)> &callback) {
        _packet_callbacks[command_id] = callback;
    }

    void PacketHandler::operator()(Packet &packet) {
        const uint16_t command_id = packet.getCommandId();
        if (!_packet_callbacks.contains(command_id)) {
            std::cerr << "Command id: " << command_id << " not implemented 1" << std::endl;
            return;
        }
        _packet_callbacks[command_id](packet, 0);
    }

    void PacketHandler::operator()(Packet &packet, const uint16_t client_id) {
        const uint16_t command_id = packet.getCommandId();
        if (!_packet_callbacks.contains(command_id)) {
            std::cerr << "Command id: " << command_id << " not implemented 2" << std::endl;
            return;
        }
        _packet_callbacks[command_id](packet, client_id);
    }
} // Network
