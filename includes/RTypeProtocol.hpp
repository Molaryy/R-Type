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
#define PLAYER_HEALTH 100

#define PLAYER_BULLET_RATE 15
#define PLAYER_BULLET_SPEED 5
#define PLAYER_BULLET_SIZE 15

#define TURRET_BULLET_SPEED 1
#define TURRET_BULLET_SIZE 15
#define TURRET_BULLET_RATE 40

#define TURRET_HEALTH 4
#define TURRET_SIZE 30

#define FLY_SPEED 1
#define FLY_SIZE 20
#define FLY_HEALTH 2

#define BOSS_HEART_SIZE 50
#define BOSS_HEART_HEALTH 5
#define BOSS_HEART_BULLET_RATE 60

#define BOSS_SIZE 50
#define BOSS_HEALTH 10
#define BOSS_BULLET_RATE 20

#define SERVER_TPS 30

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
        PLAYER_BULLET,
        ENEMY_FLY,
        ENEMY_TURRET,
        ENEMY_BULLET,
        BOSS_HEART,
        BOSS,
        WALL,
    };

    enum LobbyState : uint8_t {
        OPEN, // Lobby is open and waiting to start, you can connect
        FULL, // Lobby is full and waiting to start, you can't connect
        IN_GAME, // Lobby is in game, you can't connect
        CLOSE, // Lobby is closed, you can't connect
    };

    enum CommandIdClient : uint16_t {
        CONNECT, // EmptyPacket, Client send this packet at startup
        DISCONNECT, // EmptyPacket, Disconnection

        ASK_LOBBY_LIST, // EmptyPacket, Ask for number of lobby
        ASK_LOBBY_DATA, // AskLobbyDataPacket, Ask for data of a lobby id

        JOIN_LOBBY_BY_ID, // JoinLobbyPacket, Ask to join a lobby by id -> Server will respond with ACCEPT_LOBBY_JOIN if success
        JOIN_NEW_LOBBY, // EmptyPacket, Ask to join a newly created lobby -> Server will respond with ACCEPT_LOBBY_JOIN
        JOIN_RANDOM_LOBBY, // EmptyPacket, Join a random open lobby, or create a new one if all are closed -> Server will respond with ACCEPT_LOBBY_JOIN
        LEAVE_LOBBY, // EmptyPacket, Leave actual lobby

        ASK_START_GAME, // EmptyPacket, Ask for game start, will start game for the actual lobby

        INPUT_KEYS, // PacketInputsKeysPacket, all inputs from clients
    };

    enum CommandIdServer : uint16_t {
        ACCEPT_CONNECTION, // EmptyPacket, response to Client CONNECT, server acceptinig this client

        LOBBY_LIST, // LobbyListPacket, Send number of lobbys
        LOBBY_DATA, // LobbyDataPacket, Send data about lobby id
        ACCEPT_LOBBY_JOIN, //AcceptLobbyJoinPacket, server send this after receiving JOIN_LOBBY_BY_ID, JOIN_NEW_LOBBY, JOIN_LOBBY_RANDOM
        START_GAME, // EmptyPacket, Game is starting, server will start sending game state
        END_GAME, // EmptyPacket, Game is over

        SPAWN, // SpawnEntityPacket, data about a new entity to be displayed
        HIT, // HitPacket, entity got hit
        KILL, // DeadPacket, entity is dead because 0 helth or out of screen
        POSITION_VELOCITY, // EntityPositionVelocityPacket, state of a new entity on screen

        SERVER_SHUTDOWN, // EmptyPacket, server death
    };

    struct EmptyPacket {
        uint8_t empty = 0; // In c++, empty structures size is 1 byte, and this is the best way to ensure that the empty struct is null-initialized
    };

    struct Vector2f {
        float x;
        float y;
    };

    struct SpawnEntityPacket {
        std::size_t entity_id; // Server id for entity
        EntityType type; // Type of entity
        Vector2f position; // Position at creation
        Vector2f velocity; // Velocity at creation
    };

    struct EntityPositionVelocityPacket {
        std::size_t entity_id; // Server id of entity
        Vector2f position; // New position
        Vector2f velocity; // New velocity
    };

    struct HitPacket {
        std::size_t entity_id; // Server id of hitted entity
        int new_health; // New health of hitted entity
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

    struct JoinLobbyPacket {
        std::size_t lobby_id; // Lobby id to join
    };

    struct AcceptLobbyJoinPacket {
        std::size_t lobby_id; // ID of to joined lobby
        std::size_t entity_id; // Server id of the new joined client
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
