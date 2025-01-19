/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Systems.hpp
*/

#pragma once

#include <cmath>

#include "Components.hh"
#include "Components.hpp"
#include "IndexedZipper.hh"
#include "Registry.hh"
#include "Zipper.hh"
#include "entities/EnemyTank.hpp"
#include "entities/EnemyTurret.hpp"
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
        const SparseArray<Bonus> &bonuses = r.get_components<Bonus>();
        const SparseArray<Life> &lifes = r.get_components<Life>();
        const SparseArray<NetworkId> &network_ids = r.get_components<NetworkId>();
        std::queue<std::function<void()>> callbacks;

        for (auto &&[entity, input, velocity, position, clock, life, bonus] : IndexedZipper(inputs, velocities, positions, clocks, lifes, bonuses)) {
            if (!life.is_alive())
                continue;
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
                        PlayerBullet::create(r, Position(position.x + PLAYER_SIZE_X, position.y + PLAYER_SIZE_Y / 2.0f), bonus);
                    });
                }
            }
            for (auto &&[network_id, life2] : Zipper(network_ids, lifes)) {
                Protocol::EntityPositionVelocityPacket pos_vel(
                    entity,
                    {position.x, position.y},
                    {velocity.x, velocity.y});
                Network::Packet packet(
                    pos_vel,
                    Protocol::POSITION_VELOCITY
                );
                network.send(network_id.id, packet.serialize());
                if (bonus.type == Bonus::None || !life2.is_alive())
                    continue;
                pos_vel.entity_id = bonus.id;
                pos_vel.position = {position.x + PLAYER_SIZE_X, position.y};
                packet = Network::Packet(pos_vel, Protocol::POSITION_VELOCITY);
                network.send(network_id.id, packet.serialize());
            }
        }
        while (!callbacks.empty()) {
            callbacks.front()();
            callbacks.pop();
        }
    }

    inline void levelEndlessHandler(Registry &r, std::size_t &score) {
        if (score == 0) {
            const SparseArray<NetworkId> &network_ids = r.get_components<NetworkId>();
            SparseArray<Position> &positions = r.get_components<Position>();
            const SparseArray<Velocity> &velocities = r.get_components<Velocity>();
            size_t i = 100;

            for (auto &&[entity, network_id, position, velocity] : IndexedZipper(network_ids, positions, velocities)) {
                position.x = i;
                i += 50;
                position.y = 100;
                Protocol::EntityPositionVelocityPacket pos_vel(
                    entity,
                    {position.x, position.y},
                    {velocity.x, velocity.y});
                Network::Packet packet(
                    pos_vel,
                    Protocol::POSITION_VELOCITY
                );
            }
        }

        score += 1;

        if (score % 60 != 0)
            return;
        std::vector<std::pair<std::function<entity_t(Registry &)>, double>> spawn_rates{
            {EnemyFly::create, 2},
            {EnemyTurret::create, 1},
            {EnemyTank::create, 0.3}
        };
        double difficulty = static_cast<double>(score) / 500.0 * (0.8 + static_cast<double>(std::rand()) / RAND_MAX * (1.2 - 0.8)) + 0.5;
        if (difficulty > 1)
            difficulty = 1 + (difficulty - 1) * 0.5;
        if (difficulty > 3)
            difficulty = 3 + (difficulty - 3) * 0.5;
        if (difficulty > 4)
            difficulty = 4;
        for (auto &&[spawn, rate] : spawn_rates) {
            for ([[maybe_unused]] std::size_t _ : std::ranges::iota_view{static_cast<std::size_t>(0), static_cast<std::size_t>(difficulty * rate)})
                spawn(r);
        }
    }

    class LevelCampaignHandlerSystem {
    public:
        explicit LevelCampaignHandlerSystem(const std::size_t &initialPos)
            : score_(initialPos) {
        }

        void operator()(Registry &r) {
            score_ += 1;
        }

    private:
        std::size_t score_;
    };

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
        const SparseArray<Collision> collisions = r.get_components<Collision>();

        for (auto &&[id, pos, type, col] : IndexedZipper(positions, types, collisions)) {
            if (type.type != Protocol::PLAYER) {
                if (pos.x < -col.width || pos.y < -col.height || pos.x > WIDTH + col.width || pos.y > HEIGHT + col.height) {
                    Network::Packet packet(Protocol::DeadPacket(id, false), Protocol::KILL);
                    const SparseArray<NetworkId> &network_ids = r.get_components<NetworkId>();
                    for (auto &&[network_id] : Zipper(network_ids))
                        network.send(network_id.id, packet.serialize());

                    r.kill_entity(id);
                }
            } else {
                std::optional<Life> &life = r.get_entity_component<Life>(id);
                if (life && life->is_alive() && (pos.x < -15 || pos.y < -15 || pos.x > WIDTH - PLAYER_SIZE_X + 15 || pos.y > HEIGHT - PLAYER_SIZE_Y + 15)) {
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
