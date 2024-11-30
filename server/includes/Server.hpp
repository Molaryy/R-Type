/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Server.hpp
*/

#include <iostream>
#include <thread>
#include "Network.hpp"

namespace server {
    class Server {
        public:
            Server(int port, int maxClients): _network(port, maxClients) {};
            ~Server() {};
            int run();
        private:
            Network _network;
            std::thread _gameThread;
            std::thread _checkClientThread;
    };
}