/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** PlayerBullet.cpp
*/

#include "entities/PlayerBullet.hpp"

#include "Components.hh"
#include "Components.hpp"
#include "Packet.hpp"
#include "RTypeProtocol.hpp"
#include "Server.hpp"
#include "Zipper.hh"

void PlayerBullet::collision(Registry &r, const entity_t me, const entity_t other) {
    const std::optional<ComponentEntityType> otherType = r.get_components<ComponentEntityType>()[other];
    std::optional<Life> &life = r.get_components<Life>()[me];
    if (!otherType.has_value() || !life.has_value() || !life->is_alive() || otherType.value().side == ComponentEntityType::Ally
        || otherType->type == Protocol::ENEMY_BULLET)
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

entity_t PlayerBullet::create(Registry &r, const Position position, const Bonus bonus) {
    entity_t entity = r.spawn_entity();

    r.add_component(entity, Position(position));
    r.add_component(entity, Bonus(bonus));
    r.add_component(entity, Velocity(PLAYER_BULLET_SPEED, 0));
    r.add_component(entity, ComponentEntityType(Protocol::PLAYER_BULLET));
    r.add_component(entity, Life(1, 1));
    r.add_component(entity, Collision(PLAYER_BULLET_SIZE, PLAYER_BULLET_SIZE, collision));

    Network::Packet packet(
        Protocol::SpawnEntityPacket(
            entity,
            Protocol::PLAYER_BULLET,
            Protocol::Vector2f(position.x, position.y),
            Protocol::Vector2f(PLAYER_BULLET_SIZE, PLAYER_BULLET_SIZE),
            Protocol::Vector2f(PLAYER_BULLET_SPEED, 0),
            1
        ),
        Protocol::SPAWN
    );
    Network::INetworkServer &network = Server::getInstance().getNetwork();
    for (auto &&[network_id] : Zipper(r.get_components<NetworkId>()))
        network.send(network_id.id, packet.serialize());
    if (bonus.type == Bonus::TripleShot) {
        entity = r.spawn_entity();

        r.add_component(entity, Position(position));
        r.add_component(entity, Bonus(bonus));
        r.add_component(entity, Velocity(PLAYER_BULLET_SPEED, 1));
        r.add_component(entity, ComponentEntityType(Protocol::PLAYER_BULLET));
        r.add_component(entity, Life(1, 1));
        r.add_component(entity, Collision(PLAYER_BULLET_SIZE, PLAYER_BULLET_SIZE, collision));

        packet = Network::Packet(
            Protocol::SpawnEntityPacket(
                entity,
                Protocol::PLAYER_BULLET,
                Protocol::Vector2f(position.x, position.y),
                Protocol::Vector2f(PLAYER_BULLET_SIZE, PLAYER_BULLET_SIZE),
                Protocol::Vector2f(PLAYER_BULLET_SPEED, 1),
                1
            ),
            Protocol::SPAWN
        );
        for (auto &&[network_id] : Zipper(r.get_components<NetworkId>()))
            network.send(network_id.id, packet.serialize());


        entity = r.spawn_entity();

        r.add_component(entity, Position(position));
        r.add_component(entity, Bonus(bonus));
        r.add_component(entity, Velocity(PLAYER_BULLET_SPEED, -1));
        r.add_component(entity, ComponentEntityType(Protocol::PLAYER_BULLET));
        r.add_component(entity, Life(1, 1));
        r.add_component(entity, Collision(PLAYER_BULLET_SIZE, PLAYER_BULLET_SIZE, collision));

        packet = Network::Packet(
            Protocol::SpawnEntityPacket(
                entity,
                Protocol::PLAYER_BULLET,
                Protocol::Vector2f(position.x, position.y),
                Protocol::Vector2f(PLAYER_BULLET_SIZE, PLAYER_BULLET_SIZE),
                Protocol::Vector2f(PLAYER_BULLET_SPEED, -1),
                1
            ),
            Protocol::SPAWN
        );
        for (auto &&[network_id] : Zipper(r.get_components<NetworkId>()))
            network.send(network_id.id, packet.serialize());
    }
    return entity;
}
