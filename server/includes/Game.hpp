/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Game.hpp
*/

#pragma once

#include <iostream>
#include <thread>
#include <mutex>
#include <map>
#include "includes/NetworkEcs.hpp"

#define TICK_SPEED 30

class Interaction : public AInteraction
{
    public:
        Interaction() : _clientID(-1), _connect(-1), _gameID(-1) {}
        ~Interaction() = default;
        int getClientID() const { return _clientID; }
        void setClientID(const int clientID) { _clientID = clientID; }
        int getConnect() const { return _connect; }
        void setConnect(const int connect) { _connect = connect; }
        int getGameID() const { return _gameID; }
        void setGameID(const int gameID) { _gameID = gameID; }

        /**
         * @brief deserialize the interaction from a vector of char
         * 
         * @param data 
         */
        void deserializeInteraction(const std::vector<char> &data)
        {
            auto iter = data.begin();
            auto extractFromData = [&iter](auto &value)
            {
                std::memcpy(&value, &(*iter), sizeof(value));
                iter += sizeof(value);
            };
            extractFromData(_clientID);
            extractFromData(_connect);
            extractFromData(_gameID);
            extractFromData(_movement);
            extractFromData(_quit);
            extractFromData(createGame_);
        }

        /**
         * @brief serialize the interaction to a vector of char
         * 
         * @return std::vector<char> 
         */
        std::vector<char> serializeInteraction() const
        {
            std::vector<char> data;
            auto appendToData = [&data](auto &value)
            {
                data.insert(data.end(), reinterpret_cast<const char *>(&value), reinterpret_cast<const char *>(&value) + sizeof(value));
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
};

class Game {
    public:
        Game();
        ~Game() = default;
        void run();
        void addInteraction(const Interaction& interaction);
        void setGameID(const int gameID) { gameID_ = gameID; }
        int getGameID() const { return gameID_; }
        inline void setAvailableID(const int ID) {
            availableID_ = (ID > 4) ? -1 : ID;
        }
        int getAvailableID() const { return availableID_; }
        Game& operator=(const Game& gameToCompare) {
            if (this == &gameToCompare) return *this;
            tickSpeed_ = gameToCompare.tickSpeed_;
            tick_ = gameToCompare.tick_;
            gameID_ = gameToCompare.gameID_;
            interactionClient_ = gameToCompare.interactionClient_;
            functions_ = gameToCompare.functions_;
            functionsClient_ = gameToCompare.functionsClient_;
            return *this;
        }
    private:
        int tickSpeed_ = TICK_SPEED;
        int tick_ = 0;
        int gameID_ = -1;
        int availableID_ = 1;
        std::mutex _mutex;
        std::vector<Interaction> interactionClient_;
        std::vector<std::string> functions_;
        std::vector<std::string> functionsClient_;
};
