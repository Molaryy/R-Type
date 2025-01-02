/*
** EPITECH PROJECT, 2024
** AsioClient.hpp
** File description:
** AsioClient.hpp
*/
#ifndef ASIOWRAPPER_HPP_
#define ASIOWRAPPER_HPP_

#include <asio.hpp>
#include <string>

#include "../../includes/INetworkClient.hpp"

namespace Network {
    class AsioClient final : public INetworkClient {
    public:
        AsioClient();
        ~AsioClient() override;

        void connect(const std::string &ip, uint16_t port) override;
        void close() override;
        std::vector<uint8_t> getOldestPacket() override;
        void send(const std::vector<uint8_t> &packet) override;

    private:
        void _asyncReceive();

        std::thread _thread;
        std::mutex _mutex;

        asio::io_context _context;
        asio::ip::udp::socket _socket;
        asio::ip::udp::endpoint _server_endpoint;

        const std::string _packet_start = "PACKET_START";
        const std::string _packet_end = "PACKET_END";

        std::vector<uint8_t> _buffer;
    };
}

#endif // ASIOWRAPPER_HPP_
