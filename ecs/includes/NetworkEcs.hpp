/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** NetworkEcs.hpp
*/

#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cstring>

class INetwork {
    public:
        virtual int getConnected() const = 0;
        virtual int getCreateGame() const = 0;
        virtual void setCreateGame(int createGame) = 0;
        virtual void setConnected(int connected) = 0;
        virtual int getId() const = 0;
        virtual void setId(int id) = 0;
        virtual void setJoinGame(int joinGame) = 0;
        virtual int getJoinGame() const = 0;
        virtual void setGameId(int gameId) = 0;
        virtual int getGameId() const = 0;
        virtual void setGameIds(std::vector<int> gameIds) = 0;
        virtual std::vector<int> getGameIds() const = 0;
        virtual std::vector<char> serializeConnection() = 0;
        virtual void deserializeConnection(std::vector<char> data) = 0;
};

class ANetwork: public INetwork {
    public:
        ANetwork() = default;
        ~ANetwork() = default;
        int getConnected() const override {return _connected;};
        int getCreateGame() const override {return _createGame;};
        void setCreateGame(int createGame) override {_createGame = createGame;};
        void setConnected(int connected) override {_connected = connected;};
        void setId(int id) override {_id = id;};
        int getId() const override {return _id;};
        void setJoinGame(int joinGame) override {_JoinGame = joinGame;};
        int getJoinGame() const override {return _JoinGame;};
        void setGameId(int gameId) override {_gameId = gameId;};
        int getGameId() const override {return _gameId;};
        void setGameIds(std::vector<int> gameIds) override {_gameIds = gameIds;};
        std::vector<int> getGameIds() const override {return _gameIds;};
        std::vector<char> serializeConnection() override;
        void deserializeConnection(std::vector<char> data) override;

    protected:
        int _createGame;
        int _connected;
        int _JoinGame;
        int _id;
        int _gameId;
        std::vector<int> _gameIds;
};

class IInteraction {
    public:
        virtual void setInteraction(int mov, int shoot, int quit, int createGame) = 0;
        virtual int getMovement() const = 0;
        virtual int getQuit() const = 0;
        virtual int getCreateGame() const = 0;
};

class AInteraction : public IInteraction{
    public:
        AInteraction() = default;
        ~AInteraction() = default;
        void setInteraction(int mov, int shoot, int quit, int createGame) override {
            _movement = mov;
            _quit = quit;
            _createGame = createGame;
        };
        int getMovement() const override {return _movement;};
        int getQuit() const override {return _quit;};
        int getCreateGame() const override {return _createGame;};
    protected:
        int _movement = 0;
        int _quit = 0;
        int _createGame = 0;
};
