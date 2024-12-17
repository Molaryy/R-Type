/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main.cpp
*/

#include <asio.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <memory>

class Interaction {
    public:
        Interaction() : _clientID(0), _connect(0), _gameID(-1), _movement(0), _quit(0), createGame_(0) {}

        void setCreateGame(int createGame) { createGame_ = createGame; }
        void setGameID(int gameID) { _gameID = gameID; }

        std::vector<char> serializeInteraction() const {
            std::vector<char> data;
            auto appendToData = [&data](auto value) {
                data.insert(data.end(), reinterpret_cast<const char*>(&value), reinterpret_cast<const char*>(&value) + sizeof(value));
            };
            appendToData(_clientID);
            appendToData(_connect);
            appendToData(_gameID);
            appendToData(_movement);
            appendToData(_quit);
            appendToData(createGame_);
            return data;
        }

    private:
        int _clientID;
        int _connect;
        int _gameID;
        int _movement;
        int _quit;
        int createGame_;
};


class Client {
    public:
        Client(asio::io_context& ioContext, const std::string& host, unsigned short port)
            : ioContext_(ioContext), socket_(ioContext) {
            asio::ip::tcp::resolver resolver(ioContext_);
            asio::connect(socket_, resolver.resolve(host, std::to_string(port)));
            std::cout << "Client connected to server." << std::endl;
        }

        void sendMessage(const std::vector<char>& data) {
            asio::write(socket_, asio::buffer(data));
            std::cout << "Sending message to server..." << std::endl;
        }

        void sendCreateGameCommand() {
            Interaction interaction;
            interaction.setCreateGame(1);
            sendMessage(interaction.serializeInteraction());
        }

        void sendJoinLobbyCommand(int gameID) {
            Interaction interaction;
            interaction.setGameID(gameID);
            sendMessage(interaction.serializeInteraction());
        }

        std::string receiveMessage() {
            asio::streambuf buffer;
            asio::read_until(socket_, buffer, '\n');
            std::istream stream(&buffer);
            std::string message;
            std::getline(stream, message);
            return message;
        }

    private:
        asio::io_context& ioContext_;
        asio::ip::tcp::socket socket_;
};

int main() {
    try {
        asio::io_context ioContext;
        Interaction interaction;
        Client client(ioContext, "0.0.0.0", 8081);

        std::cout << "Sending CREATE_GAME command..." << std::endl;
        interaction.setCreateGame(1);
        client.sendMessage(interaction.serializeInteraction());

        std::cout << "Server response: " << client.receiveMessage() << std::endl;

        std::cout << "Sending JOIN_LOBBY 1 command..." << std::endl;
        interaction.setGameID(1);
        client.sendMessage(interaction.serializeInteraction());

        std::cout << "Server response: " << client.receiveMessage() << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
