/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Shoot.cpp
*/

#include "entities/Shoot.hpp"
#include "Components.hh"
#include "Components.hpp"
#include "Packet.hpp"
#include "RTypeProtocol.hpp"
#include "Server.hpp"
#include "Zipper.hh"

void Shoot::collision(Registry &r, const entity_t me, const entity_t other) {
    const std::optional<ComponentEntityType> otherType = r.get_components<ComponentEntityType>()[other];
    if (!otherType.has_value() || otherType.value().side != ComponentEntityType::Ennemy)
        return;

    Network::Packet packet(
        Protocol::DeadPacket(me, true),
        Protocol::KILL
    );
    Network::INetworkServer &network = Server::getInstance().getNetwork();
    for (auto &&[network_id] : Zipper(r.get_components<NetworkId>()))
        network.send(network_id.id, packet.serialize());
}

entity_t Shoot::create(Registry &r, const Position &position) {
    const entity_t entity = r.spawn_entity();

    Position pos(position.x + PLAYER_SIZE, static_cast<float>(position.y + PLAYER_SIZE / 2.0));
    r.add_component(entity, Position(pos));
    r.add_component(entity, Velocity(PLAYER_BULLET_SPEED, 0));
    r.add_component(entity, ComponentEntityType(Protocol::PLAYER_BULLET));
    r.add_component(entity, Life(1, 1));
    r.add_component(entity, Collision(PLAYER_BULLET_SIZE, PLAYER_BULLET_SIZE, collision));

    Network::Packet packet(
        Protocol::SpawnEntityPacket(
            entity,
            Protocol::PLAYER_BULLET,
            Protocol::Vector2f(pos.x, pos.y),
            Protocol::Vector2f(PLAYER_BULLET_SIZE, PLAYER_BULLET_SIZE),
            Protocol::Vector2f(PLAYER_BULLET_SPEED, 0),
            1
        ),
        Protocol::SPAWN
    );
    Network::INetworkServer &network = Server::getInstance().getNetwork();
    for (auto &&[network_id] : Zipper(r.get_components<NetworkId>()))
        network.send(network_id.id, packet.serialize());
    return entity;
}
