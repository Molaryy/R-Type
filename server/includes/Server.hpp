/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Server.hpp
*/

// #include <iostream>
// #include <thread>
// #include "Network.hpp"

// namespace server {
//     class Server {
//         public:
//             Server(int port, int maxClients): _network(port, maxClients) {};
//             ~Server() {};
//             int run();
//         private:
//             Network _network;
//             std::thread _gameThread;
//             std::thread _checkClientThread;
//     };
// }

#pragma once

#include <asio.hpp>
#include <thread>
#include "Network.hpp"

namespace server {
    class Server {
    public:
        Server(asio::io_context& ioContext, int port, int maxClients)
            : _network(ioContext, port, maxClients), _ioContext(ioContext) {}

        ~Server() {
            if (_gameThread.joinable())
                _gameThread.join();
            if (_checkClientThread.joinable())
                _checkClientThread.join();
        }

        int run();

    private:
        asio::io_context &_ioContext;
        Network _network;
        std::thread _gameThread;
        std::thread _checkClientThread;

        void gameLoop();
        void monitorClients();
    };
}
