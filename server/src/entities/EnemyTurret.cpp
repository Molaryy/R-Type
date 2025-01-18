/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** EnemyTurret.cpp
*/

#include "entities/EnemyTurret.hpp"

#include <cmath>
#include <entities/EnemyBullet.hpp>

#include "Components.hh"
#include "Components.hpp"
#include "IndexedZipper.hh"
#include "Packet.hpp"
#include "Server.hpp"
#include "Zipper.hh"

void EnemyTurret::ArtificialIntelligence::operator()(Registry &r, const entity_t me) {
    tick++;

    if (tick % 100)
        return;
    const SparseArray<Position> &positions = r.get_components<Position>();
    const SparseArray<ComponentEntityType> &types = r.get_components<ComponentEntityType>();
    const std::optional<Position> position = positions[me];
    if (!position.has_value())
        return;

    Position direction{};
    double shortest_distance = -1.0;
    for (auto &&[id, type, pos] : IndexedZipper(types, positions)) {
        if (type.type != Protocol::PLAYER)
            continue;
        double distance = std::sqrt(std::pow(pos.x - position->x, 2) + std::pow(pos.y - position->y, 2));
        if (shortest_distance > distance) {
            shortest_distance = distance;
            direction = pos;
        }
    }
    if (shortest_distance == -1.0)
        return;

}

void EnemyTurret::collision(Registry &r, const entity_t me, const entity_t other) {
    const std::optional<ComponentEntityType> &otherType = r.get_components<ComponentEntityType>()[other];
    std::optional<Life> &life = r.get_components<Life>()[me];
    if (!otherType.has_value() || !life.has_value() || !life->is_alive() || otherType->side != ComponentEntityType::Ally)
        return;

    if (otherType->type == Protocol::PLAYER_BULLET)
        life->takeDamage(ENEMY_BULLET_DAMAGE);
    if (otherType->type == Protocol::PLAYER)
        life->current = 0;
    Network::Packet packet;
    if (life->is_alive()) {
        packet = Network::Packet(
            Protocol::HitPacket(me, life->current),
            Protocol::HIT
        );
    } else {
        packet = Network::Packet(
            Protocol::DeadPacket(me, true),
            Protocol::KILL
        );
    }

    Network::INetworkServer &network = Server::getInstance().getNetwork();
    for (auto &&[network_id] : Zipper(r.get_components<NetworkId>()))
        network.send(network_id.id, packet.serialize());
}

entity_t EnemyTurret::create(Registry &r) {
    const entity_t entity = r.spawn_entity();

    Position pos(WIDTH, static_cast<float>(std::rand() % (HEIGHT - TURRET_SIZE)));

    r.add_component(entity, ComponentEntityType(Protocol::ENEMY_TURRET));
    r.add_component(entity, Position(pos));
    r.add_component(entity, Velocity(CAMERA_SPEED, 0));
    r.add_component(entity, Life(TURRET_HEALTH, TURRET_HEALTH));
    r.add_component(entity, Collision(TURRET_SIZE, TURRET_SIZE, collision));
    r.add_component(entity, ::ArtificialIntelligence(ArtificialIntelligence()));

    Network::Packet packet(
        Protocol::SpawnEntityPacket(
            entity,
            Protocol::ENEMY_TURRET,
            Protocol::Vector2f(pos.x, pos.y),
            Protocol::Vector2f(TURRET_SIZE, TURRET_SIZE),
            Protocol::Vector2f(CAMERA_SPEED, 0),
            TURRET_HEALTH
        ),
        Protocol::SPAWN
    );
    Network::INetworkServer &network = Server::getInstance().getNetwork();
    for (auto &&[network_id] : Zipper(r.get_components<NetworkId>()))
        network.send(network_id.id, packet.serialize());
    return entity;
}
