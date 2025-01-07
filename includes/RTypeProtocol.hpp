/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** RTypeProtocol.hpp
*/

#pragma once

#include <cstddef>
#include <cstdint>

#define TILE_SIZE 50

#define PLAYER_SPEED 4
#define PLAYER_SIZE 30
#define PLAYER_HEALTH 1

#define PLAYER_SHOOT_RATE 15
#define PLAYER_SHOOT_SPEED 5
#define PLAYER_SHOOT_SIZE 15

#define TURRET_SHOOT_SPEED 1
#define TURRET_SHOOT_SIZE 15
#define TURRET_SHOOT_RATE 40

#define TURRET_HEALTH 4
#define TURRET_SIZE 30

#define FLY_SPEED 1
#define FLY_SIZE 20
#define FLY_HEALTH 2

#define BOSS_HEART_SIZE 50
#define BOSS_HEART_HEALTH 5
#define BOSS_HEART_SHOOT_RATE 60

#define BOSS_SIZE 50
#define BOSS_HEALTH 10
#define BOSS_SHOOT_RATE 20

#define SERVER_TPS 30.0

namespace Protocol {
    enum InputKey : uint8_t {
        MOVE_UP,
        MOVE_DOWN,
        MOVE_LEFT,
        MOVE_RIGHT,
        SHOOT,
        NB_INPUTS_KEYS
    };

    enum EntityType : uint8_t {
        PLAYER,
        PLAYER_SHOOT,
        ENEMY_FLY,
        ENEMY_TURRET,
        ENEMY_SHOOT,
        BOSS_HEART,
        BOSS,
        WALL,
    };

    enum CommandIdClient : uint16_t {
        CONNECT, // Client send this packet at startup
        CLIENT_ASK_START_GAME, // Ask for game start, will start game for the actual lobby
        INPUT_KEYS, // PacketInputsKeysPacket, all inputs from clients
        DISCONNECT, // Disconnection
    };

    enum CommandIdServer : uint16_t {
        ACCEPT_CONNECTION, // AcceptConnectionPacket, response to Client CONNECT
        START_GAME, // Game is starting, server will start sending game state
        POSITION_VELOCITY, // EntityPositionVelocityPacket, state of a new entity on screen
        SPAWN, // SpawnEntityPacket, data about a new entity to be displayed
        HIT, // HitPacket, entity got hit
        KILL, // DeadPacket, entity is dead because 0 helth or out of screen
        END_GAME, // Game is over
        SERVER_SHUTDOWN,
    };

    struct EmptyPacket {
    };

    struct Vector2i {
        int x;
        int y;
    };

    struct AcceptConnectionPacket {
        std::size_t entity_id;
    };

    struct SpawnEntityPacket {
        std::size_t entity_id;
        EntityType type;
        Vector2i position;
        Vector2i velocity;
    };

    struct EntityPositionVelocityPacket {
        std::size_t entity_id;
        Vector2i position;
        Vector2i velocity;
    };

    struct HitPacket {
        std::size_t entity_id1;
        std::size_t entity_id2;
        int health_entity1;
        int health_entity2;
    };

    struct DeadPacket {
        std::size_t entity_id;
        bool natural;
    };

    struct PacketInputsKeysPacket {
        bool input_keys[NB_INPUTS_KEYS];
    };
}
