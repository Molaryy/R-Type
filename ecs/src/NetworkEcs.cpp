/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** NetworkEcs.cpp
*/

#include "NetworkEcs.hpp"

std::vector<char> ANetwork::serializeConnection()
{
    std::vector<char> data;
    auto appendToData = [&data](auto &value)
    {
        data.insert(data.end(), reinterpret_cast<char *>(&value), reinterpret_cast<char *>(&value) + sizeof(value));
    };

    appendToData(connected_);
    appendToData(createGame_);
    appendToData(JoinGame_);
    appendToData(gameID_);

    int end = -1;
    for (int gameID : gameIDs_)
    {
        appendToData(gameID);
    }
    appendToData(end);

    return data;
}

void ANetwork::deserializeConnection(const std::vector<char> &data)
{
    auto iter = data.begin();

    auto extractFromData = [&iter](auto &value)
    {
        std::memcpy(&value, &(*iter), sizeof(value));
        iter += sizeof(value);
    };

    extractFromData(connected_);
    extractFromData(createGame_);
    extractFromData(JoinGame_);
    extractFromData(gameID_);

    gameIDs_.clear(); // Clear the vector before filling it

    while (iter != data.end())
    {
        int gameID;

        extractFromData(gameID);
        if (gameID == -1)
            break;
        gameIDs_.push_back(gameID);
    }
}
