/*
** EPITECH PROJECT, 2024
** B-CPP-500-PAR-5-2-bsrtype-arthaud.poupard
** File description:
** Network.hpp
*/

// #if defined(__linux__) || defined(__APPLE__)
//     #pragma once
//     #include <sys/socket.h>
//     #include <netinet/in.h>
//     #include <sys/types.h>
//     #include <netinet/in.h>
//     #include <arpa/inet.h>
//     #include <unistd.h>
// #endif

// #ifdef _WIN64
//     #define NOGDI
//     #define NOUSER
//     #define MMNOSOUND
//     #include <winsock2.h>
//     #include <ws2tcpip.h>
//     #include <io.h>
//     #include <process.h>
// #endif

// #include <string>
// #include <iostream>
// #include <cstring>
// #include "NetworkEcs.hpp"
// #include "Game.hpp"

// namespace server {
//     class Client {
//         public:
//             Client(struct sockaddr_in addr, int id, std::string name): _addr(addr), _id(id), _name(name), _gameId(-1) {};
//             struct sockaddr_in getAddr() const {return _addr;};
//             ~Client() {};
//             void setGameId(int gameId) {_gameId = gameId;};
//             int getId() const {return _id;};
//             std::string getName() const {return _name;};
//             int getGameId() const {return _gameId;};
//             bool operator==(const Client &other) const;
//             Client &operator=(const Client &other);
//         private:
//             struct sockaddr_in _addr;
//             int _id;
//             int _gameId = -1;
//             bool _isConnected = true;
//             const std::string _name;
//     };

//     class Connection: public ANetwork {
//         public:
//             Connection() {};
//             ~Connection() {};
//     };

//     class Network {
//         public:
//             Network(int port, int maxClients);
//             ~Network();
//             void run();
//         private:
//             int _port;
//             unsigned int _maxClients;
//             bool _isRunning = true;
//             int _fd;
//             std::vector<Client> _clients;
//             struct sockaddr_in _addr;
//             struct sockaddr_in _clientAddr;
//             socklen_t _clientAddrLen;

//             #ifdef _WIN64
//                 WSADATA _wsaData;
//             #endif
//             int fillSocket();
//             int fillAddr();
//             int bindSocket();
//             Interaction manageClient(std::vector<char> buffer, int client_id, Game *game);
//             int CreateGame(std::vector<int> idNotUsableGame);
//             std::tuple<int, Connection> handleNewConn(Connection connect);
//             std::tuple<int, Connection> handleClient(std::vector<char> buffer);
//     };
// }

#pragma once

#include <asio.hpp>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "NetworkEcs.hpp"
#include "Game.hpp"

namespace server {

    class Client : public ANetwork {
    public:
        Client(asio::ip::tcp::socket socket, int id, std::string name)
            : _socket(std::move(socket)), _id(id), _name(std::move(name)) {}

        asio::ip::tcp::socket& getSocket() { return _socket; }
        const std::string& getName() const { return _name; }
        int getId() const override { return _id; }
        void setId(int id) override { _id = id; }

    private:
        asio::ip::tcp::socket _socket;
        const std::string _name;
        int _id;
    };

    class Network {
    public:
        Network(asio::io_context& ioContext, int port, int maxClients);
        ~Network();

        void run();

    private:
        asio::io_context& _ioContext;
        asio::ip::tcp::acceptor _acceptor;
        bool _isRunning;

        int _port;
        unsigned int _maxClients;

        std::vector<std::shared_ptr<Client>> _clients;
        std::vector<std::shared_ptr<Game>> _games;
        std::map<int, std::thread> _gameThreads;

        void accept();
        void handleClient(std::shared_ptr<Client> client);
        void readFromClient(std::shared_ptr<Client> client);
        void writeToClient(std::shared_ptr<Client> client, const std::string& message);

        std::shared_ptr<Game> createGame();
    };
}
