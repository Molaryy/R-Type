/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Network.cpp
*/

// #include <vector>
// #include <algorithm>
// #include "Network.hpp"
// #include "Game.hpp"

// namespace server {
//     Network::Network(int port, int maxClients): _port(port != 0 ? port : 8081), _maxClients(maxClients != 0 ? maxClients : 4) {
//         if (fillSocket() == 84 || fillAddr() == 84 || bindSocket() == 84) {
//             throw std::invalid_argument("Error: Network creation failed");
//         }
//         std::cout << "Network created on port " << _port << " with " << _maxClients << " max clients" << std::endl;
//     }

//     Network::~Network() {
// #ifdef _WIN64
//             closesocket(_fd);
// #endif
//     }

//     void Network::run() {
//         int client;
//         int id = 0;
//         Connection connect;
//         std::vector<char> buffer(1024);
//         std::vector<std::unique_ptr<Game>> games;
//         Interaction interaction;
//         std::vector<int> idNotUsableGame;
//         std::map<int, std::thread> threads;

//         while (_isRunning) {
//             client = 0;
// #if defined(__linux__) || defined(__APPLE__)
//                 client = recvfrom(_fd, buffer.data(), buffer.size(), MSG_DONTWAIT, (struct sockaddr *)&_clientAddr, &_clientAddrLen);
// #endif
// #ifdef _WIN64
//                 client = recvfrom(_fd, buffer.data(), buffer.size(), 0, (SOCKADDR *)&_clientAddr, &_clientAddrLen);
// #endif
//             if (client < 0)
//                 continue;
//             auto[id, connect] = handleClient(buffer);
//             if (id != -1) {
//                 for (auto game = games.begin(); game != games.end(); game++) {
//                     interaction = manageClient(buffer, id, game->get());
//                 }
//                 if (interaction.getQuit() == 1) {
//                     for (auto client = _clients.begin(); client != _clients.end(); client++) {
//                         if (client->getId() == id) {
//                             _clients.erase(client);
//                             break;
//                         }
//                     }
//                 }
//                 if (connect.getCreateGame() == 1) {
//                     for (auto client = _clients.begin(); client != _clients.end(); client++) {
//                         if (client->getId() == connect.getId() && client->getGameId() == -1) {
//                             auto gameTmp = std::make_unique<Game>();
//                             gameTmp->setGameId(CreateGame(idNotUsableGame));
//                             idNotUsableGame.push_back(gameTmp->getGameId());
//                             threads[gameTmp->getGameId()] = std::thread(&Game::run, gameTmp.get());
//                             gameTmp->addInteraction(interaction);
//                             client->setGameId(gameTmp->getGameId());
//                             connect.setConnected(1);
//                             auto data = connect.serializeConnection();
//                             sendto(_fd, data.data(), data.size(), 0, (struct sockaddr *)&_clientAddr, sizeof(_clientAddr));
//                             connect.setJoinGame(gameTmp->getGameId());
//                             games.push_back(std::move(gameTmp));
//                             break;
//                         }
//                     }
//                 } else if (connect.getJoinGame() == 1 && connect.getGameId() != -1) {
//                     for (auto client = _clients.begin(); client != _clients.end(); client++) {
//                         if (client->getId() == id && client->getGameId() == -1) {
//                             for (auto game = games.begin(); game != games.end(); game++) {
//                                 if (game->get()->getGameId() == connect.getGameId() && game->get()->getAvailaibleId() != -1) {
//                                     client->setGameId(connect.getGameId());
//                                     interaction.setClientID(id);
//                                     interaction.setConnect(1);
//                                     game->get()->addInteraction(interaction);
//                                     connect.setJoinGame(-1);
//                                     connect.setConnected(1);
//                                     auto data = connect.serializeConnection();
//                                     sendto(_fd, data.data(), data.size(), 0, (struct sockaddr *)&_clientAddr, sizeof(_clientAddr));
//                                     break;
//                                 } else if (game->get()->getGameId() == connect.getGameId() && game->get()->getAvailaibleId() == -1) {
//                                     connect.setJoinGame(-1);
//                                     auto data = connect.serializeConnection();
//                                     sendto(_fd, data.data(), data.size(), 0, (struct sockaddr *)&_clientAddr, sizeof(_clientAddr));
//                                     break;
//                                 }
//                             }
//                             break;
//                         } else if (client->getId() == id && client->getGameId() != -1) {
//                             connect.setJoinGame(-1);
//                             auto data = connect.serializeConnection();
//                             sendto(_fd, data.data(), data.size(), 0, (struct sockaddr *)&_clientAddr, sizeof(_clientAddr));
//                         }
//                     }
//                 } else if (connect.getJoinGame() == 1 && connect.getGameId() == -1) {
//                     std::vector<int> idServers;
//                     for (auto game = games.begin(); game != games.end(); game++) {
//                         idServers.push_back(game->get()->getGameId());
//                     }
//                     connect.setGameIds(idServers);
//                     connect.setConnected(0);
//                     connect.setJoinGame(2);
//                     id = 0;
//                     auto data = connect.serializeConnection();
//                     sendto(_fd, data.data(), data.size(), 0, (struct sockaddr *)&_clientAddr, sizeof(_clientAddr));
//                 }
//                 if (id == 0) {
//                     interaction.setClientID(connect.getId());
//                     interaction.setConnect(1);
//                     for(auto game = games.begin(); game != games.end(); game++) {
//                         if (game->get()->getGameId() == connect.getJoinGame()) {
//                             game->get()->addInteraction(interaction);
//                         }
//                     }
//                 }
//             }
//         }
//     }

//     int Network::fillSocket() {
//         int opt = 1;

//         _fd = _maxClients;
// #if defined(__linux__) || defined(__APPLE__)
//             _fd = socket(AF_INET, SOCK_STREAM, 0);
//             if (_fd == -1) {
//                 std::cerr << "Error: socket creation failed" << std::endl;
//                 return(84);
//             }
//             socklen_t size = (socklen_t)sizeof(opt);
//             if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, size) == -1) {
//                 std::cerr << "Error: socket options failed" << std::endl;
//                 return(84);
//             }
//             if (setsockopt(_fd, SOL_SOCKET, SO_REUSEPORT, &opt, size) == -1) {
//                 std::cerr << "Error: socket options failed" << std::endl;
//                 return(84);
//             }
// #endif
// #ifdef _WIN64
//             int iResult = 0;

//             iResult = WSAStartup(MAKEWORD(2, 2), &_wsaData);
//             if (iResult != NO_ERROR) {
//                 std::cerr << "Error: WSAStartup failed" << std::endl;
//                 return(84);
//             }
//             _fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//             if (_fd == INVALID_SOCKET) {
//                 std::cerr << "Error: socket creation failed" << std::endl;
//                 closesocket(_fd);
//                 WSACleanup();
//                 return(84);
//             }
//             if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&opt), sizeof(int)) == -1) {
//                 std::cerr << "Error: socket options failed" << WSAGetLastError() << std::endl;
//                 return(84);
//             }

//             unsigned read_timeout_ms = 10;
//             setsockopt(_fd, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&read_timeout_ms), sizeof(read_timeout_ms));
// #endif
//         return 0;
//     }

//     int Network::fillAddr() {
//         std::memset(&_addr, 0, sizeof(_addr));
//         std::memset(&_clientAddr, 0, sizeof(_clientAddr));

//         _addr.sin_family = AF_INET;
//         _addr.sin_port = htons(_port);
//         _addr.sin_addr.s_addr = INADDR_ANY;
//         _clientAddrLen = sizeof(_clientAddr);
//         std::cout << "Server IP: " << inet_ntoa(_addr.sin_addr) << std::endl;
//         std::cout << "Server port: " << ntohs(_addr.sin_port) << std::endl;
//         return 0;
//     }

//     int Network::bindSocket() {
// #if defined(__linux__) || defined(__APPLE__)
//             if (bind(_fd, (struct sockaddr *)&_addr, sizeof(_addr)) == -1) {
//                 std::cerr << "Error: socket binding failed" << std::endl;
//                 return 84;
//             }
//             return 0;
// #endif
// #ifdef _WIN64
//             if (bind(_fd, (SOCKADDR *)&_addr, sizeof(_addr)) == -1) {
//                 std::cerr << "Error: socket binding failed" << std::endl;
//                 return 84;
//             }
//             return 0;
// #endif
//     }

//     Interaction Network::manageClient(std::vector<char> buffer, int client_id, Game *game) {
//         Interaction interaction;
//         bool right_game = false;

//         for (auto client = _clients.begin(); client != _clients.end(); client++) {
//             if (client->getId() == client_id && client->getGameId() == (*game).getGameId()) {
//                 right_game = true;
//                 break;
//             }
//         }
//         interaction.deserializeInteraction(buffer);
//         interaction.setClientID(client_id);
//         if (interaction.getMovement() != -1 && interaction.getCreateGame() != 1 && right_game)
//             (*game).addInteraction(interaction);
//         return interaction;
//     }

//     std::tuple<int, Connection> Network::handleClient(std::vector<char> buffer) {
//         Connection connect;

//         if (_clientAddr.sin_addr.s_addr == INADDR_ANY) {
//             std::cerr << "Error: ip or port invalid" << std::endl;
//             return std::make_tuple(-1, connect);
//         }
//         connect.deserializeConnection(buffer);
//         return handleNewConn(connect);
//     }

//     std::tuple<int, Connection> Network::handleNewConn(Connection connect) {
//         for (auto client = _clients.begin(); client != _clients.end(); client++) {
//             if (inet_ntoa(client->getAddr().sin_addr) == inet_ntoa(_clientAddr.sin_addr) && client->getAddr().sin_port == _clientAddr.sin_port) {
//                 connect.setId(client->getId());
//                 return std::make_tuple(client->getId(), connect);
//             }
//         }
//         connect.setId(_clients.size() + 1);
//         _clients.push_back(Client(_clientAddr, _clients.size() + 1, "Player " + std::to_string(_clients.size() + 1)));
//         sockaddr_in cli = _clients.back().getAddr();
//         cli = cli;
//         connect.setConnected(1);
//         std::vector<char> data = connect.serializeConnection();
//         std::cout << "New Client connected" << std::endl;
//         return std::make_tuple(0, connect);
//     }

//     bool Client::operator==(const Client &other) const {
//         return _id == other.getId() && _name == other.getName();
//     }

//     Client &Client::operator=(const Client &other) {
//         if (this != &other) {
//             _addr = other.getAddr();
//             _id = other.getId();
//         }
//         return *this;
//     }

//     int Network::CreateGame(std::vector<int> idNotUsableGame) {
//         int id = 0;

//         for (int i = 0; i < _maxClients; i++) {
//             if (std::find(idNotUsableGame.begin(), idNotUsableGame.end(), i) == idNotUsableGame.end()) {
//                 id = i;
//                 break;
//             }
//         }
//         return id;
//     }
// }

#include "Network.hpp"
#include "Game.hpp"
#include <iostream>
#include <algorithm>
#include <thread>

namespace server {

    Network::Network(asio::io_context& ioContext, int port, int maxClients)
        : _ioContext(ioContext),
          _port(port != 0 ? port : 8081),
          _maxClients(maxClients != 0 ? maxClients : 4),
          _acceptor(ioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)),
          _isRunning(true) {

        asio::ip::tcp::endpoint endpoint = _acceptor.local_endpoint();
        std::string server_ip = endpoint.address().to_string();
        std::cout << "Server started on port " << _port << " And IP " << server_ip << " with max clients: " << _maxClients << std::endl;
    }

    Network::~Network() {
        _isRunning = false;

        for (auto& [gameId, thread] : _gameThreads) {
            if (thread.joinable()) {
                thread.join();
            }
        }

        std::cout << "Server shutting down..." << std::endl;
    }

    void Network::run() {
        accept();
        _ioContext.run();
    }

    void Network::accept() {
        auto socket = std::make_shared<asio::ip::tcp::socket>(_ioContext);
        _acceptor.async_accept(*socket, [this, socket](std::error_code ec) {
            if (!ec) {
                int clientId = _clients.size() + 1;
                auto client = std::make_shared<Client>(std::move(*socket), clientId, "Player " + std::to_string(clientId));
                _clients.push_back(client);

                std::cout << "New client connected: " << client->getName() << std::endl;

                handleClient(client);
            }
            if (_isRunning) {
                accept();
            }
        });
    }

    void Network::handleClient(std::shared_ptr<Client> client) {
        std::vector<char> serializedData = client->serializeConnection();
        writeToClient(client, std::string(serializedData.begin(), serializedData.end()));
        readFromClient(client);
    }

    void Network::readFromClient(std::shared_ptr<Client> client) {
        auto buffer = std::make_shared<std::vector<char>>(1024);
        auto& socket = client->getSocket();

        asio::async_read(socket, asio::buffer(*buffer), [this, client, buffer](std::error_code ec, std::size_t length) {
            if (!ec) {
                std::string message(buffer->begin(), buffer->begin() + length);
                std::vector<char> serializedData(message.begin(), message.end());
                client->deserializeConnection(serializedData);
                Interaction interaction;
                interaction.deserializeInteraction(serializedData);

                for (auto& game : _games) {
                    if (game->getGameId() == interaction.getGameId()) {
                        game->addInteraction(interaction);
                        break;
                    }
                }

                readFromClient(client);
            } else {
                std::cerr << "Client disconnected: " << client->getName() << std::endl;
                _clients.erase(std::remove(_clients.begin(), _clients.end(), client), _clients.end());
            }
        });
    }

    void Network::writeToClient(std::shared_ptr<Client> client, const std::string& message) {
        auto buffer = std::make_shared<std::string>(message);
        auto& socket = client->getSocket();

        asio::async_write(socket, asio::buffer(*buffer), [buffer](std::error_code ec, std::size_t) {
            if (ec) {
                std::cerr << "Error sending message to client" << std::endl;
            }
        });
    }

    std::shared_ptr<Game> Network::createGame() {
        auto game = std::make_shared<Game>();
        int gameId = _games.size() + 1;

        game->setGameId(gameId);
        _games.push_back(game);

        std::cout << "Game created with ID: " << gameId << std::endl;
        _gameThreads[gameId] = std::thread(&Game::run, game);
        return game;
    }
}
