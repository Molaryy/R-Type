/*
** EPITECH PROJECT, 2024
** INetworkServer.hpp
** File description:
** INetworkServer.hpp
*/

#pragma once

#include <vector>
#include <cstdint>

namespace Network {
    class INetworkServer {
    public:
        virtual ~INetworkServer() = default;

        /**
         * @brief Start hosting a server on a port, asynchronously receiving all data
         *
         * @param port listenning
         */
        virtual void host(uint16_t port) = 0;

        /**
         * @brief Close connections and sockets.
         */
        virtual void close() = 0;

        /**
         * @brief Get all oldest full packet received for every client who send it
         *
         * @return list of oldest full packet content and client id who sended it
         */
        virtual std::vector<std::pair<uint16_t, std::vector<uint8_t>>> getAllOldestPacket() = 0;

        /**
         * @brief Asynchronously sends a message to a specific client
         *
         * @param client id of the client wich to send message
         * @param packet serialized paceket to send.
         */
        virtual void send(uint16_t client, const std::vector<uint8_t> &packet) = 0;

        /**
         * @brief Asynchronously sends a message to all connected clients
         *
         * @param packet serialized paceket to send.
         */
        virtual void sendAll(const std::vector<uint8_t> &packet) = 0;
    };
}
