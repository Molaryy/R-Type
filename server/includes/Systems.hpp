/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Systems.hpp
*/

#pragma once

#include <entities/EnemyTurret.hpp>

#include "Components.hh"
#include "Components.hpp"
#include "IndexedZipper.hh"
#include "Registry.hh"
#include "Zipper.hh"
#include "entities/EnemyFly.hpp"
#include "entities/Player.hpp"
#include "entities/PlayerBullet.hpp"

namespace Systems {
    inline void handleClientInputs(Registry &r) {
        Network::INetworkServer &network = Server::getInstance().getNetwork();

        SparseArray<ClientInputs> &inputs = r.get_components<ClientInputs>();
        SparseArray<Position> &positions = r.get_components<Position>();
        SparseArray<Velocity> &velocities = r.get_components<Velocity>();
        SparseArray<Delay> &clocks = r.get_components<Delay>();
        const SparseArray<Life> &lifes = r.get_components<Life>();
        const SparseArray<NetworkId> &network_ids = r.get_components<NetworkId>();
        std::queue<std::function<void()>> callbacks;

        for (auto &&[entity, input, velocity, position, clock, life] : IndexedZipper(inputs, velocities, positions, clocks, lifes)) {
            if (!life.is_alive())
                return;
            velocity.x = 0;
            velocity.y = 0;
            ++clock.last;
            if (!input.input_keys.empty()) {
                if (std::ranges::find(input.input_keys, Protocol::MOVE_UP) != input.input_keys.end())
                    velocity.y -= PLAYER_SPEED;
                else if (std::ranges::find(input.input_keys, Protocol::MOVE_DOWN) != input.input_keys.end())
                    velocity.y += PLAYER_SPEED;
                if (std::ranges::find(input.input_keys, Protocol::MOVE_LEFT) != input.input_keys.end())
                    velocity.x -= PLAYER_SPEED;
                else if (std::ranges::find(input.input_keys, Protocol::MOVE_RIGHT) != input.input_keys.end())
                    velocity.x += PLAYER_SPEED;
                if (std::ranges::find(input.input_keys, Protocol::SHOOT) != input.input_keys.end() && clock.delay <= clock.last) {
                    clock.last = 0;
                    callbacks.emplace([&] {
                        PlayerBullet::create(r, Position(position.x + PLAYER_SIZE_X, position.y + PLAYER_SIZE_Y / 2.0f));
                    });
                }
            }
            Network::Packet packet(
                Protocol::EntityPositionVelocityPacket(
                    entity,
                    {position.x, position.y},
                    {velocity.x, velocity.y}),
                Protocol::POSITION_VELOCITY
            );
            for (auto &&[network_id] : Zipper(network_ids))
                network.send(network_id.id, packet.serialize());
        }
        while (!callbacks.empty()) {
            callbacks.front()();
            callbacks.pop();
        }
    }

    inline void levelEndlessHandler(Registry &r, std::size_t &pos_in_level) {
        pos_in_level += 1;

        if (pos_in_level % 120)
            return;
        const int difficulty = static_cast<int>(pos_in_level / 180) + 1;
        for (int i = 0; i < difficulty / 2 + 1; ++i)
            EnemyFly::create(r);
        if (difficulty % 3 == 0)
            EnemyTurret::create(r);
    }

    inline void levelCampaignHandler(Registry &r, std::size_t &pos_in_level) {
    }

    inline void killNoHealthEntitys(Registry &r) {
        std::queue<entity_t> entity_to_kill;
        for (auto &&[id, life, type] : IndexedZipper(r.get_components<Life>(), r.get_components<ComponentEntityType>()))
            if (!life.is_alive() && type.type != Protocol::PLAYER)
                entity_to_kill.push(id);

        while (!entity_to_kill.empty()) {
            r.kill_entity(entity_to_kill.front());
            entity_to_kill.pop();
        }
    }

    inline void runArtificialIntelligence(Registry &r) {
        SparseArray<ArtificialIntelligence> &ias = r.get_components<ArtificialIntelligence>();
        std::queue<std::function<void()>> callbacks;

        for (auto &&[id, ia] : IndexedZipper(ias))
            callbacks.emplace([&r, id, &ia] {
                ia.ia(r, id);
            });
        while (!callbacks.empty()) {
            callbacks.front()();
            callbacks.pop();
        }
    }

    inline void killOutOfScreenEntity(Registry &r) {
        Network::INetworkServer &network = Server::getInstance().getNetwork();
        const SparseArray<Position> positions = r.get_components<Position>();
        const SparseArray<ComponentEntityType> types = r.get_components<ComponentEntityType>();

        for (auto &&[id, pos, type] : IndexedZipper(positions, types)) {
            if (type.type != Protocol::PLAYER) {
                if (pos.x < -50 || pos.y < -50 || pos.x > WIDTH + 50 || pos.y > HEIGHT + 50) {
                    Network::Packet packet(Protocol::DeadPacket(id, false), Protocol::KILL);
                    const SparseArray<NetworkId> &network_ids = r.get_components<NetworkId>();
                    for (auto &&[network_id] : Zipper(network_ids))
                        network.send(network_id.id, packet.serialize());

                    r.kill_entity(id);
                }
            } else {
                std::optional<Life> &life = r.get_entity_component<Life>(id);
                if (life && life->is_alive() && (pos.x < -5 || pos.y < -5 || pos.x > WIDTH - PLAYER_SIZE_X + 5 || pos.y > HEIGHT - PLAYER_SIZE_Y + 5)) {
                    life->current = 0;
                    Network::Packet packet(Protocol::DeadPacket(id, true), Protocol::KILL);
                    const SparseArray<NetworkId> &network_ids = r.get_components<NetworkId>();
                    for (auto &&[network_id] : Zipper(network_ids))
                        network.send(network_id.id, packet.serialize());
                }
            }
        }
    }

    inline void gameOver(Registry &r, const std::function<void()> &gameOverCallBack) {
        const SparseArray<NetworkId> &network_ids = r.get_components<NetworkId>();
        const SparseArray<Life> &lifes = r.get_components<Life>();

        for (auto &&[network_id, life] : Zipper(network_ids, lifes))
            if (life.is_alive())
                return;
        gameOverCallBack();
    }
}
