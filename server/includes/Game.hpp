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
#include "NetworkEcs.hpp"

#define TICK_SPEED 30

namespace server {
    class Interaction : public AInteraction {
    public:
        Interaction() : _clientId(-1), _connect(-1), _gameId(-1) {}
        ~Interaction() = default;

        int getClientID() const { return _clientId; }
        void setClientID(int clientID) { _clientId = clientID; }

        int getConnect() const { return _connect; }
        void setConnect(int connect) { _connect = connect; }

        int getGameId() const { return _gameId; }
        void setGameId(int gameId) { _gameId = gameId; }

        void deserializeInteraction(const std::vector<char> &data) {
            std::memcpy(this, data.data(), sizeof(Interaction));
        }

        std::vector<char> serializeInteraction() const {
            std::vector<char> serializedData(sizeof(Interaction));
            std::memcpy(serializedData.data(), this, sizeof(Interaction));
            return serializedData;
        }

    private:
        int _clientId;
        int _connect;
        int _gameId;
    };

    class Game {
    public:
        Game();
        ~Game() = default;

        void run();
        void addInteraction(const Interaction& interaction);

        void setGameId(int gameId) { _gameId = gameId; }
        int getGameId() const { return _gameId; }

        void setAvailaibleId(int id) {
            _avalaible_id = (id > 4) ? -1 : id;
        }
        int getAvailaibleId() const { return _avalaible_id; }

        Game& operator=(const Game& other) {
            if (this == &other) return *this;

            _tickSpeed = other._tickSpeed;
            _tick = other._tick;
            _gameId = other._gameId;
            _interaction_client = other._interaction_client;
            _functions = other._functions;
            _functions_client = other._functions_client;

            return *this;
        }

    private:
        int _tickSpeed = TICK_SPEED;
        int _tick = 0;
        int _gameId = -1;
        int _avalaible_id = 1;

        std::mutex _mutex;
        std::vector<Interaction> _interaction_client;
        std::vector<std::string> _functions;
        std::vector<std::string> _functions_client;
    };

    // class Game {
    //     public:
    //         Game();
    //         ~Game() = default;

    //         void run();
    //         void addInteraction(Interaction interaction) {
    //             _mutex.lock();
    //             _interaction_client.push_back(interaction);
    //             _mutex.unlock();
    //         }
    //         void setGameId(int gameId) {_gameId = gameId;};
    //         int getGameId() const {return _gameId;};
    //         void setAvailaibleId(int id) {
    //             if (id > 4)
    //                 _avalaible_id = -1;
    //             else
    //                 _avalaible_id = id;
    //         };
    //         int getAvailaibleId() const {return _avalaible_id;};
    //         Game &operator=(const Game &other) {
    //             if (this == &other)
    //                 return *this;

    //             _tickSpeed = other._tickSpeed;
    //             _tick = other._tick;
    //             _gameId = other._gameId;

    //             _interaction_client = other._interaction_client;
    //             _functions = other._functions;
    //             _functions_client = other._functions_client;

    //             return *this;
    //         }

    //     private:
    //         int _tickSpeed = TICK_SPEED;
    //         int _tick;
    //         std::mutex _mutex_tick_send;
    //         int _last_tick_send = 0;
    //         int _gameId;
    //         std::mutex _mutex;
    //         std::vector<Interaction> _interaction_client;
    //         std::vector<std::string> _functions;
    //         std::vector<std::string> _functions_client;
    //         std::mutex _mutex_client;
    //         int _avalaible_id = 1;
    // };
}
