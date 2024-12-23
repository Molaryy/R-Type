/*
** EPITECH PROJECT, 2024
** B-CPP-500-PAR-5-2-bsrtype-arthaud.poupard
** File description:
** Network.hpp
*/

#pragma once

#include <asio.hpp>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "NetworkEcs.hpp"
#include "Game.hpp"

class Client : public ANetwork
{
    public:
        Client(asio::ip::tcp::socket socket, int id, std::string name)
            : socket_(std::move(socket)), id_(id), name_(std::move(name)) {}
        asio::ip::tcp::socket &getSocket() { return socket_; }
        const std::string &getName() const { return name_; }
        void setId(const int id) override { id_ = id; }
        int getId() const override { return id_; }
        void setGame(Game *game) { game_ = game; }
        Game *getGame() const { return game_; }
    private:
        asio::ip::tcp::socket socket_;
        const std::string name_;
        int id_;
        Game *game_;
};

class Network
{
    public:
        Network(asio::io_context &ioContext, unsigned int port, unsigned int maxClients);
        ~Network()
        {
            isRunning_ = false;
            for (auto &[gameId, thread] : gameThreads_)
            {
                if (thread.joinable())
                {
                    thread.join();
                }
            }
            std::cout << "Server shutting down..." << std::endl;
        }
        void run();
        const std::vector<std::shared_ptr<Client>>& getClients() const { return clients_; }
        std::shared_ptr<Game> getGame(int gameId);
        const std::vector<std::shared_ptr<Game>> &getGames() const { return games_; }
        void writeToClient(const std::shared_ptr<Client> &client, const std::string &message);

        void broadcastToClients(const std::string &message)
        {
            for (auto it = clients_.begin(); it != clients_.end();) {
                try {
                    // writeToClient(*it, message);
                    ++it;
                } catch (const std::system_error &e) {
                    it = clients_.erase(it);
                }
            }
        }

    private:
        unsigned int port_;
        unsigned int maxClients_;
        bool isRunning_;
        std::vector<std::shared_ptr<Client>> clients_;
        std::vector<std::shared_ptr<Game>> games_;
        std::map<int, std::thread> gameThreads_;
        asio::io_context &ioContext_;
        asio::ip::tcp::acceptor acceptor_;
        void accept();
        void handleClient(const std::shared_ptr<Client> &client);
        void readFromClient(const std::shared_ptr<Client> &client);
        std::shared_ptr<Game> createGame();
};
