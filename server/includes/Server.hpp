/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Server.hpp
*/



#pragma once

#include "Network.hpp"
#include <asio.hpp>
#include <thread>

class Server
{
    public:
        Server(asio::io_context &ioContext, const int port, const int maxClients) : network_(ioContext, port, maxClients), ioContext_(ioContext){}
        ~Server()
        {
            if (gameThread_.joinable())
                gameThread_.join();
            if (checkClientThread_.joinable())
                checkClientThread_.join();
        }
        void run();
        void gameLoop();
        void monitorClients();
    private:
        Network network_;
        asio::io_context &ioContext_;
        std::thread gameThread_;
        std::thread checkClientThread_;
};
