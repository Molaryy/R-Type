/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Systems.hpp
*/

#pragma once

#include <entities/Shoot.hpp>

#include "Components.hh"
#include "Components.hpp"
#include "IndexedZipper.hh"
#include "Registry.hh"
#include "Zipper.hh"

namespace Systems {
    [[maybe_unused]] static void handleClientInputs(Registry &r) {
        SparseArray<ClientInputs> &inputs = r.get_components<ClientInputs>();
        SparseArray<Position> &positions = r.get_components<Position>();
        SparseArray<Velocity> &velocities = r.get_components<Velocity>();
        SparseArray<Delay> &clocks = r.get_components<Delay>();
        std::queue<std::function<void()>> callbacks;

        for (auto &&[entity, input, velocity, position, clock] : IndexedZipper(inputs, velocities, positions, clocks)) {
            velocity.x = 0;
            velocity.y = 0;
            if (input.input_keys.empty())
                continue;
            if (std::ranges::find(input.input_keys, Protocol::MOVE_UP) != input.input_keys.end())
                velocity.y -= PLAYER_SPEED;
            else if (std::ranges::find(input.input_keys, Protocol::MOVE_DOWN) != input.input_keys.end())
                velocity.y += PLAYER_SPEED;
            if (std::ranges::find(input.input_keys, Protocol::MOVE_LEFT) != input.input_keys.end())
                velocity.x -= PLAYER_SPEED;
            else if (std::ranges::find(input.input_keys, Protocol::MOVE_RIGHT) != input.input_keys.end())
                velocity.x += PLAYER_SPEED;
            if (std::ranges::find(input.input_keys, Protocol::SHOOT) != input.input_keys.end() && clock.check_activation())
                callbacks.emplace([&] {
                    Shoot::create(r, position);
                });
        }
        while (!callbacks.empty()) {
            callbacks.front()();
            callbacks.pop();
        }
    }

    [[maybe_unused]] static void levelHandler(Registry &r) {
    }

    [[maybe_unused]] static void sendGameState(Registry &r) {
        const SparseArray<Velocity> &velocities = r.get_components<Velocity>();
        const SparseArray<Life> &lifes = r.get_components<Life>();
        const SparseArray<Position> &positions = r.get_components<Position>();
        const SparseArray<ComponentEntityType> &entity_types = r.get_components<ComponentEntityType>();
        for (const auto &&[entity, velocity, position, type, life] : IndexedZipper(velocities, positions, entity_types, lifes)) {
            if (!life.is_alive() || type.type == Protocol::ENEMY_SHOOT || type.type == Protocol::PLAYER_SHOOT
                || type.type == Protocol::WALL || type.type == Protocol::ENEMY_TURRET)
                continue;

            Network::Packet packet(
                Protocol::EntityPositionVelocityPacket(
                    entity,
                    {position.x, position.y},
                    {velocity.x, velocity.y}),
                Protocol::POSITION_VELOCITY
            );
            Server::getInstance().getNetwork().sendAll(packet.serialize());
        }
    }
}
