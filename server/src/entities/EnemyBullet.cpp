/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** EnemyBullet.cpp
*/

#include "entities/EnemyBullet.hpp"

#include <cmath>

#include "Components.hh"
#include "Components.hpp"
#include "Packet.hpp"
#include "RTypeProtocol.hpp"
#include "Server.hpp"
#include "Zipper.hh"

void EnemyBullet::collision(Registry &r, const entity_t me, const entity_t other) {
    const std::optional<ComponentEntityType> otherType = r.get_components<ComponentEntityType>()[other];
    std::optional<Life> &life = r.get_components<Life>()[me];
    if (!otherType.has_value() || !life.has_value() || !life->is_alive() || otherType.value().side != ComponentEntityType::Ally)
        return;

    life->current = 0;
    Network::Packet packet(
        Protocol::DeadPacket(me, false),
        Protocol::KILL
    );
    Network::INetworkServer &network = Server::getInstance().getNetwork();
    for (auto &&[network_id] : Zipper(r.get_components<NetworkId>()))
        network.send(network_id.id, packet.serialize());
}

entity_t EnemyBullet::create(Registry &r, const Position start, const Position end, const float speed) {
    const entity_t entity = r.spawn_entity();

    const float x = end.x - start.x;
    const float y = end.y - start.y;

    const float scalling_factor = speed / sqrtf(powf(x, 2) + powf(y, 2));

    Velocity velocity(x / scalling_factor, y / scalling_factor);

    r.add_component(entity, Position(start));
    r.add_component(entity, Velocity(velocity));
    r.add_component(entity, ComponentEntityType(Protocol::ENEMY_BULLET));
    r.add_component(entity, Life(1, 1));
    r.add_component(entity, Collision(ENEMY_BULLET_SIZE, ENEMY_BULLET_SIZE, collision));

    Network::Packet packet(
        Protocol::SpawnEntityPacket(
            entity,
            Protocol::ENEMY_BULLET,
            Protocol::Vector2f(start.x, start.y),
            Protocol::Vector2f(ENEMY_BULLET_SIZE, ENEMY_BULLET_SIZE),
            Protocol::Vector2f(velocity.x, velocity.y),
            1
        ),
        Protocol::SPAWN
    );
    Network::INetworkServer &network = Server::getInstance().getNetwork();
    for (auto &&[network_id] : Zipper(r.get_components<NetworkId>()))
        network.send(network_id.id, packet.serialize());
    return entity;
}
