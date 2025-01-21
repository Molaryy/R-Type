/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** BossHeart.cpp
*/

#include "entities/BossHeart.hpp"

#include <cmath>

#include "Components.hh"
#include "Components.hpp"
#include "IndexedZipper.hh"
#include "Packet.hpp"
#include "Server.hpp"
#include "Zipper.hh"
#include "entities/BonusHealth.hpp"
#include "entities/BonusForce.hpp"
#include "entities/EnemyBullet.hpp"
#include "entities/PlayerBullet.hpp"

void BossHeart::ArtificialIntelligence::operator()(Registry &r, const entity_t me) {
    Network::INetworkServer &network = Server::getInstance().getNetwork();
    SparseArray<Position> &positions = r.get_components<Position>();
    SparseArray<Velocity> &velocities = r.get_components<Velocity>();
    std::optional<Position> &position = positions[me];
    std::optional<Velocity> &velocity = velocities[me];
    if (!position)
        return;

    if (position->x <= BOSS_HEART_STOP_POS) {
        position->x = BOSS_HEART_STOP_POS;
        velocity->x = 0;

        for (auto &&[network_id] : Zipper(r.get_components<NetworkId>())) {
            Protocol::EntityPositionVelocityPacket pos_vel(
                me,
                {position->x, position->y},
                {velocity->x, velocity->y});
            Network::Packet packet(
                pos_vel,
                Protocol::POSITION_VELOCITY
            );
            network.send(network_id.id, packet.serialize());
        }
    }

    tick++;
    if (tick % BOSS_HEART_BULLET_RATE)
        return;
    const SparseArray<ComponentEntityType> &types = r.get_components<ComponentEntityType>();

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
    EnemyBullet::create(r, Position(position->x, position->y - BOSS_HEART_SIZE / 2.f), direction, BOSS_HEART_BULLET_SPEED);
}

void BossHeart::collision(Registry &r, const entity_t me, const entity_t other) {
    const std::optional<ComponentEntityType> &otherType = r.get_components<ComponentEntityType>()[other];
    std::optional<Life> &life = r.get_components<Life>()[me];
    std::optional<Life> &otherLife = r.get_components<Life>()[other];
    const std::optional<Bonus> &otherBonus = r.get_entity_component<Bonus>(other);
    if (!otherType.has_value() || !life.has_value() || !life->is_alive() || otherType->side != ComponentEntityType::Ally)
        return;

    if (otherType->type == Protocol::PLAYER_BULLET)
        life->takeDamage(PLAYER_BULLET_DAMAGE + (otherBonus->type == Bonus::Damage ? BONUS_FORCE_DAMAGE_BOOST : 0));
    if (otherType->type == Protocol::PLAYER)
        otherLife->current = 0;
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
        if (pos)
            BonusHealth::create(r, pos.value());
    }
}

entity_t BossHeart::create(Registry &r) {
    const Position pos(WIDTH, static_cast<float>(std::rand() % HEIGHT));

    return createFromPos(r, pos);
}

entity_t BossHeart::createFromPos(Registry &r, const Position &position) {
    Position pos(position);

    const entity_t entity = r.spawn_entity();

    Velocity vel(CAMERA_SPEED, 0);

    r.add_component(entity, ComponentEntityType(Protocol::BOSS_HEART));
    r.add_component(entity, Position(pos));
    r.add_component(entity, Velocity(vel));
    r.add_component(entity, Life(BOSS_HEART_HEALTH, BOSS_HEART_HEALTH));
    r.add_component(entity, Collision(BOSS_HEART_SIZE, BOSS_HEART_SIZE, collision));
    r.add_component(entity, ::ArtificialIntelligence(ArtificialIntelligence()));

    Network::Packet packet(
        Protocol::SpawnEntityPacket(
            entity,
            Protocol::BOSS_HEART,
            Protocol::Vector2f(pos.x, pos.y),
            Protocol::Vector2f(BOSS_HEART_SIZE, BOSS_HEART_SIZE),
            Protocol::Vector2f(vel.x, vel.y),
            BOSS_HEART_HEALTH
        ),
        Protocol::SPAWN
    );
    Network::INetworkServer &network = Server::getInstance().getNetwork();
    for (auto &&[network_id] : Zipper(r.get_components<NetworkId>()))
        network.send(network_id.id, packet.serialize());
    return entity;
}
