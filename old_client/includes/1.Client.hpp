/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Client.hpp
*/

#pragma once

#include <asio.hpp>
#include <thread>
#include <vector>
#include "ClientNetwork.hpp"

class Client {
    public:
        Client(const std::string &serverIP, unsigned int port);
        ~Client();

        void run();
        void stop();
        void sendInteraction(const Interaction &interaction);

    private:
        asio::io_context ioContext_;
        ClientNetwork network_;
        std::thread ioThread_;
};
