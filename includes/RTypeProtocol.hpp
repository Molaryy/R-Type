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

    enum LobbyState : uint8_t {
        OPEN, // Lobby is open and waiting to start, you can connect
        FULL, // Lobby is full and waiting to start, you can't connect
        IN_GAME,  // Lobby is in game, you can't connect
    };

    enum CommandIdClient : uint16_t {
        CONNECT, // EmptyPacket, Client send this packet at startup
        CLIENT_ASK_START_GAME, // EmptyPacket, Ask for game start, will start game for the actual lobby
        INPUT_KEYS, // PacketInputsKeysPacket, all inputs from clients
        ASK_LOBBY_LIST, // EmptyPacket, Ask for number of lobby
        ASK_LOBBY_DATA, // AskLobbyDataPacket, Ask for data of a lobby id
        JOIN_LOBBY_BY_ID, // AskLobbyDataPacket, Ask to join a lobby by id
        DISCONNECT, // EmptyPacket, Disconnection
    };

    enum CommandIdServer : uint16_t {
        ACCEPT_CONNECTION, // AcceptConnectionPacket, response to Client CONNECT
        START_GAME, // Game is starting, server will start sending game state
        POSITION_VELOCITY, // EntityPositionVelocityPacket, state of a new entity on screen
        SPAWN, // SpawnEntityPacket, data about a new entity to be displayed
        HIT, // HitPacket, entity got hit
        KILL, // DeadPacket, entity is dead because 0 helth or out of screen
        END_GAME, // EmptyPacket, Game is over
        LOBBY_LIST, // LobbyListPacket, Send number of lobbys
        LOBBY_DATA, // LobbyDataPacket, Send data about lobby id
        SERVER_SHUTDOWN,
    };

    struct EmptyPacket {
    };

    struct Vector2i {
        int x;
        int y;
    };

    struct AcceptConnectionPacket {
        std::size_t entity_id; // Your server id for the incoming game
    };

    struct SpawnEntityPacket {
        std::size_t entity_id; // Server id for entity
        EntityType type; // Type of entity
        Vector2i position; // Position at creation
        Vector2i velocity; // Velocity at creation
    };

    struct EntityPositionVelocityPacket {
        std::size_t entity_id; // Server id of entity
        Vector2i position; // New position
        Vector2i velocity; // New velocity
    };

    struct HitPacket {
        std::size_t entity_id1; // Server id of first collided entity
        std::size_t entity_id2; // Server id of second collided entity
        int health_entity1; // New health of first entity
        int health_entity2; // New health of second entity
    };

    struct DeadPacket {
        std::size_t entity_id; // Server id of dead entity
        bool natural; // Is death natural ? (death from getting hit = true, death from out of screen = false)
    };

    struct AskLobbyDataPacket {
        std::size_t lobby_id; // Lobby id to ask data
    };

    struct LobbyListPacket {
        std::size_t number_of_lobbys; // Number of lobbys
    };

    struct JoinLobbyById {
        std::size_t lobby_id; // Lobby id to join
    };

    struct LobbyDataPacket {
        std::size_t lobby_id; // Lobby id of data
        LobbyState lobby_state; // State of lobby
        uint8_t nb_players; // Numbers of players in lobby
    };

    struct InputsKeysPacket {
        bool input_keys[NB_INPUTS_KEYS]; // Array of boolean with each index representing if the key is pressed
    };
}
