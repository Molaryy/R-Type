/*
** EPITECH PROJECT, 2024
** INetworkClient.hpp
** File description:
** INetworkClient.hpp
*/

#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace Network {
    class INetworkClient {
    public:
        virtual ~INetworkClient() = default;

        /**
         * @brief Connects to a remote server with ip and port and start receiving asynchronously data received, putting it in a buffer
         *
         * @param ip address of the remote server.
         * @param port on the remote ip to connect to.
         */
        virtual void connect(const std::string &ip, uint16_t port) = 0;

        /**
         * @brief Closes the connection and the socket
         */
        virtual void close() = 0;

        /**
         * @brief Get the oldest full packet received
         *
         * @return oldest full packet content
         */
        virtual std::vector<uint8_t> getOldestPacket() = 0;

        /**
         * @brief Asynchronously sends a message to the connected endpoint
         *
         * @param packet serialized paceket to send.
         */
        virtual void send(const std::vector<uint8_t> &packet) = 0;
    };
}
