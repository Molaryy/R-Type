/*
** EPITECH PROJECT, 2024
** Asio.cpp
** File description:
** Asio.cpp
*/

#include "AsioClient.hpp"

#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
#define LIB_EXPORT __declspec(dllexport)
#else
#define LIB_EXPORT
#endif

namespace Network {
    AsioClient::AsioClient(): _socket(_context) {
    }

    AsioClient::~AsioClient() {
        close();
    }

    void AsioClient::connect(const std::string &ip, const uint16_t port) {
        try {
            _server_endpoint = asio::ip::udp::endpoint(asio::ip::make_address_v4(ip), port);
            _socket.open(asio::ip::udp::v4());
        } catch (const std::exception &e) {
            std::cerr << ip << e.what() << std::endl;
        }
        std::thread([this]() {
            _asyncReceive();
        }).detach();
    }

    void AsioClient::close() {
        _socket.close();
    }

    std::vector<uint8_t> AsioClient::getOldestPacket() {
        std::vector<uint8_t> packet;

        std::lock_guard lock(_mutex);
        auto it_end = std::ranges::search(_buffer, _packet_end);
        if (it_end.empty())
            return packet;
        auto it_start = std::ranges::search(_buffer, _packet_start);
        if (!it_start.empty())
            packet = std::vector(
                _buffer.begin() + std::distance(_buffer.begin(), it_start.begin()) + static_cast<long int>(_packet_start.length()),
                _buffer.begin() + std::distance(_buffer.begin(), it_end.begin())
            );
        _buffer.erase(
            _buffer.begin(),
            _buffer.begin() + std::distance(_buffer.begin(), it_end.begin()) + static_cast<long int>(_packet_end.length())
        );
        return packet;
    }

    void AsioClient::send(const std::vector<uint8_t> &packet) {
        std::vector<uint8_t> data(_packet_start.begin(), _packet_start.end());
        data.insert(data.end(), packet.begin(), packet.end());
        data.insert(data.end(), _packet_end.begin(), _packet_end.end());

        _socket.async_send_to(
            asio::buffer(data),
            _server_endpoint,
            []([[maybe_unused]] std::error_code err, [[maybe_unused]] std::size_t bytes) {
            }
        );
    }

    void AsioClient::_asyncReceive() {
        while (true) {
            std::array<uint8_t, 1024> buffer{};
            asio::error_code error;
            const size_t length = _socket.receive_from(asio::buffer(buffer), _server_endpoint, 0, error);

            if (error)
                break;

            std::lock_guard lock(_mutex);
            _buffer.resize(_buffer.size() + length);
            std::memcpy(_buffer.data() + (_buffer.size() - length), buffer.data(), length);
        }
    }

    extern "C" {
    LIB_EXPORT INetworkClient *create_instance() {
        return new AsioClient();
    }
    }
} // R_Type
