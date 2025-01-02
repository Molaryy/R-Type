/*
** EPITECH PROJECT, 2024
** Asio.hpp
** File description:
** Asio.hpp
*/
#ifndef ASIOWRAPPER_HPP_
#define ASIOWRAPPER_HPP_

#include <asio.hpp>
#include <string>
#include <chrono>
#include "../../includes/INetworkServer.hpp"

namespace Network {
    class AsioServer final : public INetworkServer {
    public:
        AsioServer();
        ~AsioServer() override;

        void host(uint16_t port) override;
        void close() override;
        std::vector<std::pair<uint16_t, std::vector<uint8_t>>> getAllOldestPacket() override;
        void send(uint16_t client, const std::vector<uint8_t> &packet) override;
        void sendAll(const std::vector<uint8_t> &packet) override;

    private:
        void _asyncReceive();

        std::mutex _mutex;

        const std::string _packet_start = "PACKET_START";
        const std::string _packet_end = "PACKET_END";

        asio::ip::udp::endpoint _tmp_endpoint;
        asio::io_context _context;
        asio::ip::udp::socket _socket;
        std::vector<std::pair<asio::ip::udp::endpoint, std::vector<uint8_t>>> _clients{};
    };
}

#endif // ASIOWRAPPER_HPP_
