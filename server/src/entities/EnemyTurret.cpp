/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** EnemyTurret.cpp
*/

#include "entities/EnemyTurret.hpp"

#include <cmath>
#include <entities/BonusHealth.hpp>
#include <entities/BonusTripleShot.hpp>

#include "entities/BonusForce.hpp"
#include "Components.hh"
#include "Components.hpp"
#include "IndexedZipper.hh"
#include "Packet.hpp"
#include "Server.hpp"
#include "Zipper.hh"
#include "entities/EnemyBullet.hpp"
#include "entities/PlayerBullet.hpp"

void EnemyTurret::ArtificialIntelligence::operator()(Registry &r, const entity_t me) {
    tick++;

    if (tick % TURRET_BULLET_RATE)
        return;
    const SparseArray<Position> &positions = r.get_components<Position>();
    const SparseArray<ComponentEntityType> &types = r.get_components<ComponentEntityType>();
    const std::optional<Position> &position = positions[me];
    if (!position.has_value())
        return;

    Position direction{};
    double shortest_distance = -1.0;
    for (auto &&[id, type, pos] : IndexedZipper(types, positions)) {
        if (type.type != Protocol::PLAYER)
            continue;
        double distance = std::sqrt(std::pow(pos.x - position->x, 2) + std::pow(pos.y - position->y, 2));
        if (shortest_distance > distance || shortest_distance == -1.0) {
            shortest_distance = distance;
            direction = pos;
        }
    }
    if (shortest_distance == -1.0)
        return;
    EnemyBullet::create(r, position.value(), direction, TURRET_BULLET_SPEED);
}

void EnemyTurret::collision(Registry &r, const entity_t me, const entity_t other) {
    const std::optional<ComponentEntityType> &otherType = r.get_components<ComponentEntityType>()[other];
    std::optional<Life> &life = r.get_components<Life>()[me];
    const std::optional<Bonus> &otherBonus = r.get_entity_component<Bonus>(other);
    if (!otherType.has_value() || !life.has_value() || !life->is_alive() || otherType->side != ComponentEntityType::Ally)
        return;

    if (otherType->type == Protocol::PLAYER_BULLET)
        life->takeDamage(PLAYER_BULLET_DAMAGE + (otherBonus->type == Bonus::Damage ? BONUS_FORCE_DAMAGE_BOOST : 0));
    if (otherType->type == Protocol::PLAYER)
        life->current = 0;
    if (life->is_alive()) {
        Network::Packet packet(
            Protocol::HitPacket(me, life->current),
            Protocol::HIT
        );
        Network::INetworkServer &network = Server::getInstance().getNetwork();
        for (auto &&[network_id] : Zipper(r.get_components<NetworkId>()))
            network.send(network_id.id, packet.serialize());
    } else {
        const std::optional<Position> &pos = r.get_entity_component<Position>(me);
        if (pos) {
            if (Server::random(TURRET_DROP_BONUS_HEALTH_CHANCE))
                BonusHealth::create(r, pos.value());
            else if (Server::random(TURRET_DROP_BONUS_DAMAGE_CHANCE))
                BonusForce::create(r, pos.value());
            else if (Server::random(TURRET_DROP_BONUS_TRIPLE_CHANCE))
                BonusTripleShot::create(r, pos.value());
        }
    }
}

entity_t EnemyTurret::create(Registry &r) {
    const Position pos(WIDTH, static_cast<float>(std::rand() % (HEIGHT - TURRET_SIZE)));

    return createFromPos(r, pos);
}

entity_t EnemyTurret::createFromPos(Registry &r, const Position &position) {
    Position pos(position);

    const entity_t entity = r.spawn_entity();

    const float x = 0 - pos.x;
    const float y = static_cast<float>(std::rand() % (HEIGHT - TURRET_SIZE)) - pos.y;

    const float scalling_factor = static_cast<float>(std::abs(CAMERA_SPEED + TURRET_SPEED)) / sqrtf(powf(x, 2) + powf(y, 2));

    Velocity vel(x * scalling_factor, y * scalling_factor);

    r.add_component(entity, ComponentEntityType(Protocol::ENEMY_TURRET));
    r.add_component(entity, Position(pos));
    r.add_component(entity, Velocity(vel));
    r.add_component(entity, Life(TURRET_HEALTH, TURRET_HEALTH));
    r.add_component(entity, Collision(TURRET_SIZE, TURRET_SIZE, collision));
    r.add_component(entity, ::ArtificialIntelligence(ArtificialIntelligence()));

    Network::Packet packet(
        Protocol::SpawnEntityPacket(
            entity,
            Protocol::ENEMY_TURRET,
            Protocol::Vector2f(pos.x, pos.y),
            Protocol::Vector2f(TURRET_SIZE, TURRET_SIZE),
            Protocol::Vector2f(vel.x, vel.y),
            TURRET_HEALTH
        ),
        Protocol::SPAWN
    );
    Network::INetworkServer &network = Server::getInstance().getNetwork();
    for (auto &&[network_id] : Zipper(r.get_components<NetworkId>()))
        network.send(network_id.id, packet.serialize());
    return entity;
}
