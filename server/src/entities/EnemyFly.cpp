/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** EnemyFly.cpp
*/

#include "entities/EnemyFly.hpp"

#include "Components.hh"
#include "Components.hpp"
#include "Packet.hpp"
#include "Server.hpp"
#include "Zipper.hh"

void EnemyFly::collision(Registry &r, const entity_t me, const entity_t other) {
    const std::optional<ComponentEntityType> otherType = r.get_components<ComponentEntityType>()[other];
    std::optional<Life> &life = r.get_components<Life>()[me];
    if (!otherType.has_value() || !life.has_value() || otherType->side != ComponentEntityType::Ally)
        return;

    if (otherType->type == Protocol::PLAYER_BULLET)
        life->takeDamage(10);
    if (otherType->type == Protocol::PLAYER)
        life->current = 0;
    Network::Packet packet;
    if (life->is_alive())
        packet = Network::Packet(
            Protocol::HitPacket(me, life->current),
            Protocol::HIT
        );
    else
        packet = Network::Packet(
           Protocol::DeadPacket(me, true),
           Protocol::KILL
       );

    Network::INetworkServer &network = Server::getInstance().getNetwork();
    for (auto &&[network_id] : Zipper(r.get_components<NetworkId>()))
        network.send(network_id.id, packet.serialize());
}

entity_t EnemyFly::create(Registry &r) {
    const entity_t entity = r.spawn_entity();

    Position pos(WIDTH, static_cast<float>(std::rand() % HEIGHT));

    r.add_component(entity, Position(pos));
    r.add_component(entity, Velocity(0, 0));
    r.add_component(entity, Life(FLY_HEALTH, FLY_HEALTH));
    r.add_component(entity, ComponentEntityType(Protocol::ENEMY_FLY));
    r.add_component(entity, Collision(FLY_SIZE, FLY_SIZE, collision));

    Network::Packet packet(
        Protocol::SpawnEntityPacket(
            entity,
            Protocol::ENEMY_FLY,
            Protocol::Vector2f(pos.x, pos.y),
            Protocol::Vector2f(FLY_SIZE, FLY_SIZE),
            Protocol::Vector2f(-FLY_SPEED, 0),
            FLY_HEALTH
        ),
        Protocol::SPAWN
    );
    Network::INetworkServer &network = Server::getInstance().getNetwork();
    for (auto &&[network_id] : Zipper(r.get_components<NetworkId>()))
        network.send(network_id.id, packet.serialize());
    return entity;
}
