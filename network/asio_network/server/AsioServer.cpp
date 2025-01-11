/*
** EPITECH PROJECT, 2024
** AsioServer.cpp
** File description:
** AsioServer.cpp
*/

#include "AsioServer.hpp"

#include <iostream>
#include <ranges>

#if defined(_WIN32) || defined(_WIN64)
#define LIB_EXPORT __declspec(dllexport)
#else
#define LIB_EXPORT
#endif

namespace Network {
    AsioServer::AsioServer()
        : _socket(_context) {
    }

    AsioServer::~AsioServer() = default;

    void AsioServer::host(const uint16_t port) {
        _socket = asio::ip::udp::socket(_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port));
        _asyncReceive();
        std::thread([this] {
            _context.run();
        }).detach();
    }

    void AsioServer::close() {
        _context.stop();
    }

    std::vector<std::pair<uint16_t, std::vector<uint8_t>>> AsioServer::getAllOldestPacket() {
        std::vector<std::pair<uint16_t, std::vector<uint8_t>>> packets;

        int i = -1;
        for (auto &data : _clients | std::views::values) {
            std::lock_guard lock(_mutex);
            i++;
            auto it_end = std::ranges::search(data, _packet_end);
            if (it_end.empty())
                continue;
            auto it_start = std::ranges::search(data, _packet_start);
            if (!it_start.empty())
                packets.emplace_back(
                    static_cast<uint16_t>(i),
                    std::vector(
                        data.begin() + std::distance(data.begin(), it_start.begin()) + static_cast<long int>(_packet_start.length()),
                        data.begin() + std::distance(data.begin(), it_end.begin())
                    )
                );
            data.erase(
                data.begin(),
                data.begin() + std::distance(data.begin(), it_end.begin()) + static_cast<long int>(_packet_end.length())
            );
        }
        return packets;
    }

    void AsioServer::send(const uint16_t client, const std::vector<uint8_t> &packet) {
        if (client >= _clients.size()) {
            std::cerr << "Client " << client << " not found" << std::endl;
            return;
        }

        std::vector<uint8_t> data(_packet_start.begin(), _packet_start.end());
        data.insert(data.end(), packet.begin(), packet.end());
        data.insert(data.end(), _packet_end.begin(), _packet_end.end());

        _socket.async_send_to(
            asio::buffer(data),
            _clients[client].first,
            []([[maybe_unused]] std::error_code err, [[maybe_unused]] std::size_t bytes) {
            }
        );
    }

    void AsioServer::sendAll(const std::vector<uint8_t> &packet) {
        std::vector<uint8_t> data(_packet_start.begin(), _packet_start.end());
        data.insert(data.end(), packet.begin(), packet.end());
        data.insert(data.end(), _packet_end.begin(), _packet_end.end());

        for (auto &endpoint : std::views::keys(_clients)) {
            _socket.async_send_to(
                asio::buffer(data),
                endpoint,
                []([[maybe_unused]] std::error_code err, [[maybe_unused]] std::size_t bytes) {
                }
            );
        }
    }

    void AsioServer::_asyncReceive() {
        auto buffer = std::make_shared<std::array<char, 1024>>();

        _socket.async_receive_from(
            asio::buffer(*buffer), _tmp_endpoint,
            [this, buffer](const std::error_code ec, const std::size_t length) {
                if (ec) {
                    std::cerr << "DEBUG: async_receive_from encountered an error: " << ec.message() << std::endl;
                    return;
                } {
                    std::lock_guard lock(_mutex);
                    auto it = std::ranges::find_if(
                        _clients,
                        [this](const std::pair<asio::ip::udp::endpoint, std::vector<uint8_t>> &client) {
                            return client.first == _tmp_endpoint;
                        });
                    if (it == _clients.end()) {
                        _clients.emplace_back(_tmp_endpoint, std::vector<uint8_t>());
                        it = _clients.end() - 1;
                    }
                    it->second.resize(it->second.size() + length);
                    std::memcpy(it->second.data() + (it->second.size() - length), buffer->data(), length);
                }
                _asyncReceive();
            });
    }

    extern "C" {
    LIB_EXPORT INetworkServer *create_instance() {
        return new AsioServer();
    }
    }
} // R_Type
