/*
** EPITECH PROJECT, 2025
** B-YEP-400_R-Type
** File description:
** RTypeProtocol.hpp
*/

#pragma once

#include <cstdint>
#include <cstddef>

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
        CONNECT,
        CLIENT_ASK_START_GAME,
        INPUT_KEYS,
        DISCONNECT,
    };

    enum CommandIdServer : uint16_t {
        ACCEPT_CONNECTION,
        START_GAME,
        POSITION_VELOCITY,
        SPAWN,
        HIT,
        KILL,
        CAMERA_MOVE,
        END_GAME,
        SERVER_SHUTDOWN,
    };

    struct Empty {
    };

    struct Vector2i {
        int x;
        int y;
    };

    struct AcceptConnection {
        std::size_t entity_id;
    };

    struct SpawnEntity {
        std::size_t entity_id;
        EntityType type;
        Vector2i position;
        Vector2i velocity;
    };

    struct EntityPositionVelocity {
        std::size_t entity_id;
        Vector2i position;
        Vector2i velocity;
    };

    struct Hit {
        std::size_t entity_id1;
        std::size_t entity_id2;
    };

    struct Dead {
        std::size_t entity_id;
        bool natural;
    };

    struct CameraMove {
        Vector2i position;
        Vector2i velocity;
    };

    struct PacketInputsKeys {
        bool input_keys[NB_INPUTS_KEYS];
    };
}
