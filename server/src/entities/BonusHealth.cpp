/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** BonusHealth.cpp
*/

#include "entities/BonusHealth.hpp"

#include "Components.hh"
#include "Components.hpp"
#include "Packet.hpp"
#include "RTypeProtocol.hpp"
#include "Server.hpp"
#include "Zipper.hh"

void BonusHealth::collision(Registry &r, const entity_t me, const entity_t other) {
    const std::optional<ComponentEntityType> otherType = r.get_components<ComponentEntityType>()[other];
    std::optional<Life> &life = r.get_components<Life>()[me];
    if (!otherType.has_value() || !life.has_value() || !life->is_alive() || otherType->type != Protocol::PLAYER)
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

entity_t BonusHealth::create(Registry &r, const Position position) {
    const entity_t entity = r.spawn_entity();

    r.add_component(entity, ComponentEntityType(Protocol::BONUS_HEALTH));
    r.add_component(entity, Position(position));
    r.add_component(entity, Velocity(CAMERA_SPEED, 0));
    r.add_component(entity, Life(1, 1));
    r.add_component(entity, Collision(BONUS_HEALTH_SIZE, BONUS_HEALTH_SIZE, collision));

    Network::Packet packet(
        Protocol::SpawnEntityPacket(
            entity,
            Protocol::BONUS_HEALTH,
            Protocol::Vector2f(position.x, position.y),
            Protocol::Vector2f(BONUS_HEALTH_SIZE, BONUS_HEALTH_SIZE),
            Protocol::Vector2f(CAMERA_SPEED, 0),
            1
        ),
        Protocol::SPAWN
    );
    Network::INetworkServer &network = Server::getInstance().getNetwork();
    for (auto &&[network_id] : Zipper(r.get_components<NetworkId>()))
        network.send(network_id.id, packet.serialize());
    return entity;
}
