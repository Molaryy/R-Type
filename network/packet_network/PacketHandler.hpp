/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** PacketHandler.hpp
*/

#pragma once

#include <cstdint>
#include <functional>

#include "Packet.hpp"

namespace Network {
    /**
     * @class PacketHandler
     * @brief Handles the callbacks for packets
     */
    class PacketHandler {
    public:
        /**
         * @brief Constructs a new PacketHandler object.
         */
        PacketHandler();

        /**
         * @brief Destroys the PacketHandler object.
         */
        ~PacketHandler();


        /**
         * @brief Registers a callback for a specific packet command ID.
         *
         * This overload allows the callback to access only the packet.
         *
         * @param command_id The command ID to associate with the callback.
         * @param callback A function to be invoked when a packet with the given command ID is received.
         */
        void setPacketCallback(uint16_t command_id, const std::function<void (Packet &)> &callback);

        /**
         * @brief Registers a callback for a specific packet command ID.
         *
         * This overload allows the callback to access the packet and the client ID.
         *
         * @param command_id The command ID to associate with the callback.
         * @param callback A function to be invoked when a packet with the given command ID is received.
         */
        void setPacketCallback(uint16_t command_id, const std::function<void (Packet &, uint16_t)> &callback);


        /**
         * @brief Invokes the appropriate callback for the given packet.
         *
         * @param packet The packet to process.
         */
        void operator()(Packet &packet);


        /**
         * @brief Invokes the appropriate callback for the given packet and client ID.
         *
         * @param packet The packet to process.
         * @param client_id Client id who sended the packet
         */
        void operator()(Packet &packet, uint16_t client_id);

    private:
        std::unordered_map<uint16_t, std::function<void (Packet &, uint16_t)>> _packet_callbacks;
    };
} // Network
