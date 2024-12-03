/*
** EPITECH PROJECT, 2024
** B-CPP-500-PAR-5-2-bsrtype-arthaud.poupard
** File description:
** Network.hpp
*/

#if defined(__linux__) || defined(__APPLE__)
    #pragma once
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <sys/types.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

#ifdef _WIN64
    #define NOGDI
    #define NOUSER
    #define MMNOSOUND
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <io.h>
    #include <process.h>
#endif

#include <string>
#include <iostream>
#include <cstring>
#include "NetworkEcs.hpp"
#include "Game.hpp"

namespace server {
    class Client {
        public:
            Client(struct sockaddr_in addr, int id, std::string name): _addr(addr), _id(id), _name(name), _gameId(-1) {};
            struct sockaddr_in getAddr() const {return _addr;};
            ~Client() {};
            void setGameId(int gameId) {_gameId = gameId;};
            int getId() const {return _id;};
            std::string getName() const {return _name;};
            int getGameId() const {return _gameId;};
            bool operator==(const Client &other) const;
            Client &operator=(const Client &other);
        private:
            struct sockaddr_in _addr;
            int _id;
            int _gameId = -1;
            bool _isConnected = true;
            const std::string _name;
    };

    class Connection: public ANetwork {
        public:
            Connection() {};
            ~Connection() {};
    };

    class Network {
        public:
            Network(int port, int maxClients);
            ~Network();
            void run();
        private:
            int _port;
            unsigned int _maxClients;
            bool _isRunning = true;
            int _fd;
            std::vector<Client> _clients;
            struct sockaddr_in _addr;
            struct sockaddr_in _clientAddr;
            socklen_t _clientAddrLen;

            #ifdef _WIN64
                WSADATA _wsaData;
            #endif
            int fillSocket();
            int fillAddr();
            int bindSocket();
            Interaction manageClient(std::vector<char> buffer, int client_id, Game *game);
            int CreateGame(std::vector<int> idNotUsableGame);
            std::tuple<int, Connection> handleNewConn(Connection connect);
            std::tuple<int, Connection> handleClient(std::vector<char> buffer);
    };
}
