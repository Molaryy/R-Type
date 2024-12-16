/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Network.hpp
*/

#pragma once

#include <asio.hpp>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "../../server/includes/Game.hpp"

class ClientNetwork {
    public:
        ClientNetwork(asio::io_context &ioContext, const std::string &serverIP, unsigned int port);
        ~ClientNetwork();

        void connect();
        void sendData(const std::vector<char> &data);
        void receiveData();

    private:
        asio::io_context &ioContext_;
        asio::ip::tcp::socket socket_;
        asio::ip::tcp::resolver resolver_;
        asio::ip::tcp::resolver::results_type endpoints_;
        void handleReceive(const std::vector<char> &data);
};
