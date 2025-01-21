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
    if (!otherType.has_value() || !life.has_value() || !life->is_alive() || otherType.value().side != ComponentEntityType::Enemy
        || otherType->type == Protocol::ENEMY_BULLET)
        return;

    life->current = -1;
}

entity_t PlayerBullet::create(Registry &r, const Position position, const Velocity velocity, const Bonus bonus) {
    const entity_t entity = r.spawn_entity();

    r.add_component(entity, Position(position));
    r.add_component(entity, Velocity(velocity));
    r.add_component(entity, ComponentEntityType(Protocol::PLAYER_BULLET));
    r.add_component(entity, Bonus(bonus));
    r.add_component(entity, Life(1, 1));
    r.add_component(entity, Collision(PLAYER_BULLET_SIZE, PLAYER_BULLET_SIZE, collision));

    Network::Packet packet(
        Protocol::SpawnEntityPacket(
            entity,
            Protocol::PLAYER_BULLET,
            Protocol::Vector2f(position.x, position.y),
            Protocol::Vector2f(PLAYER_BULLET_SIZE, PLAYER_BULLET_SIZE),
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

entity_t PlayerBullet::create(Registry &r, const Position position, const Bonus bonus) {
    const entity_t e = create(r, position, Velocity(PLAYER_BULLET_SPEED, 0), bonus);
    if (bonus.type == Bonus::TripleShot) {
        create(r, position, Velocity(PLAYER_BULLET_SPEED, 5), bonus);
        create(r, position, Velocity(PLAYER_BULLET_SPEED, -5), bonus);
    }
    return e;
}
