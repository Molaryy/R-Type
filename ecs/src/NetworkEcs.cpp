/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** NetworkEcs.cpp
*/

#include "NetworkEcs.hpp"

std::vector<char> ANetwork::serializeConnection() {
    std::vector<char> data;
    data.insert(data.end(), reinterpret_cast<char*>(&_connected), reinterpret_cast<char*>(&_connected) + sizeof(_connected));
    data.insert(data.end(), reinterpret_cast<char*>(&_createGame), reinterpret_cast<char*>(&_createGame) + sizeof(_createGame));
    data.insert(data.end(), reinterpret_cast<char*>(&_JoinGame), reinterpret_cast<char*>(&_JoinGame) + sizeof(_JoinGame));
    data.insert(data.end(), reinterpret_cast<char*>(&_gameId), reinterpret_cast<char*>(&_gameId) + sizeof(_gameId));
    int end = -1;

    for (int id : _gameIds) {
        data.insert(data.end(), reinterpret_cast<char*>(&id), reinterpret_cast<char*>(&id) + sizeof(id));
    }
    data.insert(data.end(), reinterpret_cast<char*>(&end), reinterpret_cast<char*>(&end) + sizeof(end));
    return data;
}

void ANetwork::deserializeConnection(std::vector<char> data) {
    int size = 0;

    std::memcpy(&_connected, data.data() + size, sizeof(_connected));
    size += sizeof(_connected);
    std::memcpy(&_createGame, data.data() + size, sizeof(_createGame));
    size += sizeof(_createGame);
    std::memcpy(&_JoinGame, data.data() + size, sizeof(_JoinGame));
    size += sizeof(_JoinGame);
    std::memcpy(&_gameId, data.data() + size, sizeof(_gameId));
    size += sizeof(_gameId);
    while (size < data.size() && data[size] != -1) {
        int gameId;

        if (size < data.size() && data[size] != -1) {
            std::memcpy(&gameId, data.data() + size, sizeof(gameId));
            _gameIds.push_back(gameId);
            size += sizeof(gameId);
        }
    }
}
