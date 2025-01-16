/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Player.cpp
*/

#include "entities/Player.hpp"

#include "Zipper.hh"
#include "Components.hh"
#include "Components.hpp"
#include "Packet.hpp"
#include "Server.hpp"

void Player::collision(Registry &r, const entity_t me, const entity_t other) {
    const std::optional<ComponentEntityType> otherType = r.get_components<ComponentEntityType>()[other];
    std::optional<Life> &optLife = r.get_components<Life>()[me];
    if (!otherType.has_value() || !optLife.has_value() || otherType.value().side == ComponentEntityType::Ally)
        return;

    Life &life = optLife.value();
    life.takeDamage(10);

    Network::Packet packet(
        Protocol::HitPacket(me, life.current),
        Protocol::HIT
    );
    Network::INetworkServer &network = Server::getInstance().getNetwork();
    for (auto &&[network_id] : Zipper(r.get_components<NetworkId>()))
        network.send(network_id.id, packet.serialize());
}

entity_t Player::create(Registry &r, const std::uint16_t client_id) {
    const entity_t entity = r.spawn_entity();

    r.add_component(entity, ClientInputs());
    r.add_component(entity, NetworkId(client_id));
    r.add_component(entity, Position(0, 0));
    r.add_component(entity, Velocity(0, 0));
    r.add_component(entity, Delay(PLAYER_BULLET_RATE, PLAYER_BULLET_RATE));
    r.add_component(entity, Life(PLAYER_HEALTH, PLAYER_HEALTH));
    r.add_component(entity, ComponentEntityType(Protocol::PLAYER));
    r.add_component(entity, Collision(PLAYER_SIZE, PLAYER_SIZE, collision));

    return entity;
}
