/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Network.cpp
*/

#include "Network.hpp"
#include "Game.hpp"
#include <iostream>
#include <algorithm>
#include <thread>
#include <vector>

/**
 * @brief Construct a new Network:: Network object
 * 
 * @param ioContext 
 * @param port 
 * @param maxClients 
 */
Network::Network(asio::io_context& ioContext, unsigned int port, unsigned int maxClients) :
    ioContext_(ioContext), port_(port != 0 ? port : 8081),
    maxClients_(maxClients != 0 ? maxClients : 4),
    acceptor_(ioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)),
    isRunning_(true)
{
    asio::ip::tcp::endpoint endpoint = acceptor_.local_endpoint();
    std::string server_ip = endpoint.address().to_string();
    std::cout << "Server started on port " << port_ << " And IP " << server_ip << " with max clients: " << maxClients_ << std::endl;

    auto game = createGame();
    game->setGameID(1);
    games_.push_back(game);
}

/**
 * @brief Destroy the Network:: Network object
 * 
 */
void Network::run()
{
    accept();
    ioContext_.run();
}

std::shared_ptr<Game> Network::getGame(int gameId) {
    for (const auto &game : games_) {
        if (game->getGameID() == gameId) {
            return game;
        }
    }
    return nullptr;
}

/**
 * @brief accept incoming connections
 * 
 */
void Network::accept()
{
    auto socket = std::make_shared<asio::ip::tcp::socket>(ioContext_);
    acceptor_.async_accept(*socket, [this, socket](const std::error_code errorCode)
    {
        if (!errorCode)
        {
            std::size_t clientID = clients_.size() + 1;
            const auto client = std::make_shared<Client>(std::move(*socket), clientID, "Player " + std::to_string(clientID));
            clients_.push_back(client);
            std::cout << "New client connected: " << client->getName() << std::endl;
            handleClient(client);
        }
        if (isRunning_)
            accept();
    });
}

/**
 * @brief Receive and send data to client
 * 
 * @param client 
 */
void Network::handleClient(const std::shared_ptr<Client> &client)
{
    std::vector<char> serializedData = client->serializeConnection();
    readFromClient(client);
}

/**
 * @brief Read data from client & deserialize it
 * 
 * @param client 
 */
void Network::readFromClient(const std::shared_ptr<Client>& client)
{
    auto buffer = std::make_shared<std::vector<char>>(1024);
    auto &socket = client->getSocket();

    std::cout << "Preparing to read from client: " << client->getName() << std::endl;
    asio::async_read(socket, asio::buffer(*buffer), [this, client, buffer](std::error_code errorCode, std::size_t length)
    {
        std::cout << "Async read callback triggered!" << std::endl;
        if (!errorCode)
        {
            std::vector<char> serializedData(buffer->begin(), buffer->begin() + length);
            client->deserializeConnection(serializedData);
            Interaction interaction;
            interaction.deserializeInteraction(serializedData);

            std::cout << "Deserialized Interaction: CreateGame=" << interaction.getCreateGame() << ", GameID=" << interaction.getGameID() << std::endl;

            if (interaction.getCreateGame() == 1) {
                auto newGameLobby = createGame();
                newGameLobby->addInteraction(interaction);
                client->setGame(newGameLobby.get());
                newGameLobby->addClient(client);
                writeToClient(client, "Lobby " + std::to_string(newGameLobby->getGameID()) + " created !");
            } else if (interaction.getGameID() != -1) {
                auto game = getGame(interaction.getGameID());

                if (game) {
                    game->addInteraction(interaction);
                    client->setGame(game.get());
                    game->addClient(client);
                    writeToClient(client, "Joined lobby " + std::to_string(interaction.getGameID()));
                } else {
                    writeToClient(client, "Lobby not found !");
                }
            }
            if (client->getGame()) {
                client->getGame()->addInteraction(interaction);
            }
            readFromClient(client);
        } else {
            std::cerr << "Client disconnected: " << client->getName() << std::endl;
            if (client->getGame()) {
                client->getGame()->removeClient(client);
            }
        }
    });
}

/**
 * @brief Write data to client & serialize it
 * 
 * @param client 
 * @param message 
 */
void Network::writeToClient(const std::shared_ptr<Client> &client, const std::string &message)
{
    std::cout << "Sending message to client: '" << message << "'" << std::endl;
    asio::write(client->getSocket(), asio::buffer(message + "\n"));
}

/**
 * @brief Create a new game
 * 
 * @return std::shared_ptr<Game> 
 */
std::shared_ptr<Game> Network::createGame()
{
    auto game = std::make_shared<Game>();
    int gameID = games_.size() + 1;
    game->setGameID(gameID);
    games_.push_back(game);
    std::cout << "Game created with ID: " << gameID << std::endl;
    gameThreads_[gameID] = std::thread(&Game::run, game);

    return game;
}
