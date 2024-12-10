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
        virtual ~INetwork() = default;
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
        virtual void deserializeConnection(const std::vector<char> &data) = 0;
};

class ANetwork: public INetwork
{
    public:
        ANetwork() = default;
        ~ANetwork() = default;
        int getConnected() const override {return connected_;};
        int getCreateGame() const override {return createGame_;};
        void setCreateGame(int createGame) override {createGame_ = createGame;};
        void setConnected(int connected) override {connected_ = connected;};
        void setId(int id) override {ID_ = id;};
        int getId() const override {return ID_;};
        void setJoinGame(int joinGame) override {JoinGame_ = joinGame;};
        int getJoinGame() const override {return JoinGame_;};
        void setGameId(int gameId) override {gameID_ = gameId;};
        int getGameId() const override {return gameID_;};
        void setGameIds(std::vector<int> gameIds) override {gameIDs_ = gameIds;};
        std::vector<int> getGameIds() const override {return gameIDs_;};
        std::vector<char> serializeConnection() override;
        void deserializeConnection(const std::vector<char> &data) override;

    protected:
        int connected_;
        int createGame_;
        int JoinGame_;
        int ID_;
        int gameID_;
        std::vector<int> gameIDs_;
};

class IInteraction
{
    public:
        virtual ~IInteraction() = default;
        virtual void setInteraction(int mov, int shoot, int quit, int createGame) = 0;
        virtual int getMovement() const = 0;
        virtual int getQuit() const = 0;
        virtual int getCreateGame() const = 0;
};

class AInteraction : public IInteraction
{
    public:
        AInteraction() = default;
        ~AInteraction() = default;
        void setInteraction(int mov, int shoot, int quit, int createGame) override {
            _movement = mov;
            _quit = quit;
            createGame_ = createGame;
        };
        int getMovement() const override {return _movement;};
        int getQuit() const override {return _quit;};
        int getCreateGame() const override {return createGame_;};
    protected:
        int _movement = 0;
        int _quit = 0;
        int createGame_ = 0;
};
