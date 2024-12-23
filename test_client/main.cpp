/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main.cpp
*/

#include <asio.hpp>
#include <iostream>
#include <vector>
#include <cstring>
#include <thread>
#include "Game.hpp"

class TestClient {
public:
    TestClient(const std::string &host, const std::string &port)
        : socket_(io_context_) {
        asio::ip::tcp::resolver resolver(io_context_);
        asio::connect(socket_, resolver.resolve(host, port));
        std::cout << "Connected to server at " << host << ":" << port << std::endl;
    }

    void sendInteraction(int clientID, Interaction::InteractionType type, int gameID = -1, float x = 0.0f, float y = 0.0f) {
        std::vector<char> data = serializeInteraction(clientID, type, gameID, x, y);
        asio::write(socket_, asio::buffer(data));
    }

    void readServerResponse() {
        char buffer[1024];
        size_t len = socket_.read_some(asio::buffer(buffer));
        std::cout << "Received from server: " << std::string(buffer, len) << std::endl;
    }

private:
    asio::io_context io_context_;
    asio::ip::tcp::socket socket_;

    std::vector<char> serializeInteraction(int clientID, Interaction::InteractionType type, int gameID = -1, float x = 0.0f, float y = 0.0f)
    {
        std::vector<char> data;
        auto appendToData = [&data](auto &value) {
            data.insert(data.end(), reinterpret_cast<const char *>(&value),
                        reinterpret_cast<const char *>(&value) + sizeof(value));
        };

        appendToData(clientID);
        appendToData(type);

        if (type == Interaction::CREATE_GAME || type == Interaction::JOIN_GAME) {
            appendToData(gameID);
        } else if (type == Interaction::SEND_MOVEMENT) {
            appendToData(x);
            appendToData(y);
        }

        return data;
    }
};

int main() {
    try {
        TestClient client("127.0.0.1", "8081");

        client.sendInteraction(1, Interaction::CREATE_GAME);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        client.readServerResponse();
        // for (int i = 0; i < 5; ++i) {
        //     client.sendInteraction(1, Interaction::SEND_MOVEMENT, -1, 10.0f * i, 5.0f * i);
        //     std::this_thread::sleep_for(std::chrono::milliseconds(500));
        //     client.readServerResponse();
        // }

    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
