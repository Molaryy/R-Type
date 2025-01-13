/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Components.hpp
*/

#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>

#include "RTypeProtocol.hpp"

class ComponentEntityType {
public:
    enum Side {
        Ally,
        Ennemy,
        Neutral
    };

    explicit ComponentEntityType(const Protocol::EntityType type)
        : type(type) {
        side = typeSides_.at(type);
    }

    void log() const {
        std::cout << "EntityType = { " << typeName_.at(type) << " }";
    }

    Protocol::EntityType type;
    Side side;

private:
    std::unordered_map<Protocol::EntityType, Side> typeSides_{
        {Protocol::PLAYER, Ally},
        {Protocol::PLAYER_SHOOT, Ally},
        {Protocol::ENEMY_FLY, Ennemy},
        {Protocol::ENEMY_TURRET, Ennemy},
        {Protocol::ENEMY_SHOOT, Ennemy},
        {Protocol::BOSS_HEART, Ennemy},
        {Protocol::WALL, Neutral},
    };

    std::unordered_map<Protocol::EntityType, std::string> typeName_{
        {Protocol::PLAYER, "Player"},
        {Protocol::PLAYER_SHOOT, "Player Shoot"},
        {Protocol::ENEMY_FLY, "Enemy Fly"},
        {Protocol::ENEMY_TURRET, "Enemy Turret"},
        {Protocol::ENEMY_SHOOT, "Enemy Shoot"},
        {Protocol::BOSS_HEART, "Boss Heart"},
        {Protocol::WALL, "Wall"},
    };
};

class ClientInputs {
public:
    ClientInputs() = default;

    explicit ClientInputs(const bool new_input_keys[Protocol::NB_INPUTS_KEYS]) {
        setInputs(new_input_keys);
    }

    void setInputs(const bool new_input_keys[Protocol::NB_INPUTS_KEYS]) {
        input_keys.clear();
        for (int8_t i = 0; i < Protocol::NB_INPUTS_KEYS; i++) {
            if (!new_input_keys[i])
                continue;
            input_keys.push_back(static_cast<Protocol::InputKey>(i));
        }
    }

    void log() const {
        std::cout << "ClientInputs = { ";
        for (const Protocol::InputKey key : input_keys) {
            if (key != input_keys.front())
                std::cout << ", ";
            std::cout << static_cast<unsigned int>(key);
        }
        std::cout << " }";
    }

    std::vector<Protocol::InputKey> input_keys;
};
