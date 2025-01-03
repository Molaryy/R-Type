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
#include "Registry.hh"
#include "Components.hh"
#include "Systems.hh"

class Client;

#define TICK_SPEED 30

class Interaction : public AInteraction
{
    public:

        Interaction() : clientID_(-1), _connect(-1), gameID_(-1) {}
        ~Interaction() = default;
        int getClientID() const { return clientID_; }
        void setClientID(const int clientID) { clientID_ = clientID; }
        int getConnect() const { return _connect; }
        void setConnect(const int connect) { _connect = connect; }
        int getGameID() const { return gameID_; }
        void setGameID(const int gameID) { gameID_ = gameID; }
        InteractionType getType() const { return type_; }
        void setType(const InteractionType type) { type_ = type; }
        std::pair<float, float> getMovements() const { return movement_; }
        float getMovementX() const { return movement_.first; }
        float getMovementY() const { return movement_.second; }
        void setMovement(float x, float y) { movement_ = {x, y}; }

        /**
         * @brief deserialize the interaction from a vector of char
         * 
         * @param data 
         */
        void deserializeInteraction(const std::vector<char> &data) {
            auto iter = data.begin();
            auto extractFromData = [&iter](auto &value) {
                std::memcpy(&value, &(*iter), sizeof(value));
                iter += sizeof(value);
            };
            extractFromData(clientID_);
            extractFromData(type_);
            if (type_ == CREATE_GAME || type_ == JOIN_GAME)
            // extractFromData(createGame_);
                extractFromData(gameID_);
            else if (type_ == SEND_MOVEMENT) {
                extractFromData(movement_.first);
                extractFromData(movement_.second);
            }
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
            appendToData(clientID_);
            appendToData(type_);
            if (type_ == CREATE_GAME || type_ == JOIN_GAME)
            // appendToData(createGame_);
                appendToData(gameID_);
            else if (type_ == SEND_MOVEMENT) {
                appendToData(movement_.first);
                appendToData(movement_.second);
            }
            return data;
        }
    private:
        int clientID_;
        int _connect;
        int gameID_;
        InteractionType type_;
        std::pair<float, float> movement_;
};

// struct Entity {
//     int id;
//     std::string type;
//     float x, y;
//     float velocityX, velocityY;
// };

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
        // const std::vector<Entity> &getEntities() const { return entities_; }
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
        registry &getRegistry() { return reg_; }
        void addClient(const std::shared_ptr<Client> &client) {
            clients_.push_back(client);
        }
        void removeClient(const std::shared_ptr<Client> &client) {
            clients_.erase(std::remove(clients_.begin(), clients_.end(), client), clients_.end());
        }

        const std::vector<std::shared_ptr<Client>> &getClients() const {
            return clients_;
        }
        std::string serializeGameState() const;
        void processPlayerMovement(const Interaction &interaction);

    private:
        int tickSpeed_ = TICK_SPEED;
        int tick_ = 0;
        int gameID_ = -1;
        int availableID_ = 1;
        std::mutex _mutex;
        std::vector<Interaction> interactionClient_;
        std::vector<std::string> functions_;
        std::vector<std::string> functionsClient_;
        registry reg_;
        Systems systems_;
        std::vector<std::shared_ptr<Client>> clients_;
};
