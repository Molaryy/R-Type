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
#include <functional>

#include "Registry.hh"
#include "RTypeProtocol.hpp"

class ComponentEntityType {
public:
    enum Side {
        Ally,
        Enemy,
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
        {Protocol::PLAYER_BULLET, Ally},
        {Protocol::ENEMY_FLY, Enemy},
        {Protocol::ENEMY_TURRET, Enemy},
        {Protocol::ENEMY_BULLET, Enemy},
        {Protocol::BOSS_HEART, Enemy},
        {Protocol::WALL, Neutral},
        {Protocol::BONUS_HEALTH, Neutral},
        {Protocol::BONUS_DAMAGE, Neutral},
        {Protocol::BONUS_TRIPLE_SHOT, Neutral},
    };

    std::unordered_map<Protocol::EntityType, std::string> typeName_{
        {Protocol::PLAYER, "Player"},
        {Protocol::PLAYER_BULLET, "Player Bullet"},
        {Protocol::ENEMY_FLY, "Enemy Fly"},
        {Protocol::ENEMY_TURRET, "Enemy Turret"},
        {Protocol::ENEMY_BULLET, "Enemy Bullet"},
        {Protocol::BOSS_HEART, "Boss Heart"},
        {Protocol::WALL, "Wall"},
        {Protocol::BONUS_HEALTH, "Bonus Health"},
        {Protocol::BONUS_DAMAGE, "Bonus Damage"},
        {Protocol::BONUS_TRIPLE_SHOT, "Bonus Triple_shot"},
    };
};

struct NetworkId {
    uint16_t id;

    void log() const {
        std::cout << "NetworkId = { " << id << " }";
    }
};

class ClientInputs {
public:
    ClientInputs() = default;

    explicit ClientInputs(const std::vector<Protocol::InputKey> &key_pressed) {
        setInputs(key_pressed);
    }

    void setInputs(const std::vector<Protocol::InputKey> &key_pressed) {
        input_keys.clear();
        input_keys = key_pressed;
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

struct ArtificialIntelligence {
    std::function<void (Registry &, entity_t)> ia;

    void operator()(Registry &r, const entity_t me) const {
        ia(r, me);
    }

    void log() const {
        std::cout << "ArtificalIntelligence";
    }
};

struct Bonus {
    enum BonusType {
        None,
        Damage,
        TripleShot,
    };

    BonusType type;
    entity_t id;

    void log() const {
        std::cout << "Bonus = { " << id << " : " << type << " }";
    }
};
