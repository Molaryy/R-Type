/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Client.cpp
*/

#include "Client.hpp"

Client::Client(const std::string &serverIP, unsigned int port): network_(ioContext_, serverIP, port) {}

Client::~Client() {
    stop();
}

void Client::run() {
    std::cout << "Starting client..." << std::endl;
    network_.connect();
    ioThread_ = std::thread([this]() { ioContext_.run(); });
    if (ioThread_.joinable())
        ioThread_.join();
}

void Client::stop() {
    ioContext_.stop();
    if (ioThread_.joinable())
        ioThread_.join();
}

void Client::sendInteraction(const Interaction &interaction) {
    std::vector<char> serializedData = interaction.serializeInteraction();
    network_.sendData(serializedData);
}
