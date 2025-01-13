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

void Shoot::collision(Registry &r, const entity_t me, const entity_t other) {
    const std::optional<ComponentEntityType> otherType = r.get_components<ComponentEntityType>()[other];
    std::optional<Life> &optLife = r.get_components<Life>()[me];
    if (!otherType.has_value() || !optLife.has_value() || otherType.value().side == ComponentEntityType::Ally)
        return;

    Life &life = optLife.value();
    life.takeDamage(1);

    if (!life.is_alive()) {
        Network::Packet packet(
            Protocol::DeadPacket(me, true),
            Protocol::KILL
        );
        Server::getInstance().getNetwork().sendAll(packet.serialize());
        r.kill_entity(me);
    }
}

entity_t Shoot::create(Registry &r, const Position &position) {
    const entity_t entity = r.spawn_entity();

    r.add_component(entity, Position(position));
    r.add_component(entity, Velocity(PLAYER_SHOOT_SPEED, 0));
    r.add_component(entity, ComponentEntityType(Protocol::PLAYER_SHOOT));
    r.add_component(entity, Life(1, 1));
    r.add_component(entity, Collision(PLAYER_SHOOT_SIZE, PLAYER_SHOOT_SIZE, collision));

    Network::Packet packet(
        Protocol::SpawnEntityPacket(
            entity,
            Protocol::PLAYER_SHOOT,
            Protocol::Vector2f(position.x, position.y),
            Protocol::Vector2f(PLAYER_SHOOT_SPEED, 0)
        ),
        Protocol::SPAWN
    );
    Server::getInstance().getNetwork().sendAll(packet.serialize());

    return entity;
}
