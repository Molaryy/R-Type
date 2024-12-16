/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ClientNetwork.cpp
*/

#include "ClientNetwork.hpp"

ClientNetwork::ClientNetwork(asio::io_context &ioContext, const std::string &serverIP, unsigned int port)
    : ioContext_(ioContext),
      socket_(ioContext),
      resolver_(ioContext),
      endpoints_(resolver_.resolve(serverIP, std::to_string(port))) {}

ClientNetwork::~ClientNetwork() {
    if (socket_.is_open()) {
        socket_.close();
    }
}

void ClientNetwork::connect() {
    asio::async_connect(socket_, endpoints_, [this](std::error_code ec, asio::ip::tcp::endpoint) {
        if (!ec) {
            std::cout << "Connected to server!" << std::endl;
            receiveData();
        } else {
            std::cerr << "Connection failed: " << ec.message() << std::endl;
        }
    });
}

void ClientNetwork::sendData(const std::vector<char> &data) {
    asio::async_write(socket_, asio::buffer(data), [](std::error_code ec, std::size_t) {
        if (ec) {
            std::cerr << "Failed to send data: " << ec.message() << std::endl;
        }
    });
}

void ClientNetwork::receiveData() {
    auto buffer = std::make_shared<std::vector<char>>(1024);
    asio::async_read(socket_, asio::buffer(*buffer), [this, buffer](std::error_code ec, std::size_t length) {
        if (!ec) {
            std::vector<char> data(buffer->begin(), buffer->begin() + length);
            handleReceive(data);
            receiveData();
        } else {
            std::cerr << "Disconnected from server: " << ec.message() << std::endl;
        }
    });
}

void ClientNetwork::handleReceive(const std::vector<char> &data) {
    Interaction interaction;
    interaction.deserializeInteraction(data);

    std::cout << "Received interaction: " << interaction.getMovement() << std::endl;

    // Handle interactions
}
