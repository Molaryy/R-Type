/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** EnemyFly.cpp
*/

#include "entities/EnemyFly.hpp"

#include "entities/BonusTripleShot.hpp"
#include "entities/BonusForce.hpp"
#include "entities/BonusHealth.hpp"
#include "Components.hh"
#include "Components.hpp"
#include "Packet.hpp"
#include "Server.hpp"
#include "Zipper.hh"
#include "entities/PlayerBullet.hpp"

void EnemyFly::ArtificialIntelligence::operator()(Registry &r, const entity_t me) {
    tick++;

    std::optional<Velocity> &vel = r.get_entity_component<Velocity>(me);
    const std::optional<Position> pos = r.get_entity_component<Position>(me);
    if (pos.has_value() && vel.has_value()) {
        const std::size_t atm = tick % FLY_ZIGZAG_SIZE;
        if (atm == 0 || atm == FLY_ZIGZAG_SIZE / 2) {
            vel->y = -vel->y;
            Network::Packet packet(Protocol::EntityPositionVelocityPacket(me, {pos->x, pos->y}, {vel->x, vel->y}),
                                   Protocol::POSITION_VELOCITY);
            Network::INetworkServer &network = Server::getInstance().getNetwork();
            for (auto &&[network_id] : Zipper(r.get_components<NetworkId>()))
                network.send(network_id.id, packet.serialize());
        }
    }
}

void EnemyFly::collision(Registry &r, const entity_t me, const entity_t other) {
    const std::optional<ComponentEntityType> &otherType = r.get_entity_component<ComponentEntityType>(other);
    const std::optional<Bonus> &otherBonus = r.get_entity_component<Bonus>(other);
    std::optional<Life> &life = r.get_entity_component<Life>(me);
    if (!otherType.has_value() || !life.has_value() || !life->is_alive() || otherType->side != ComponentEntityType::Ally)
        return;

    if (otherType->type == Protocol::PLAYER_BULLET)
        life->takeDamage(PLAYER_BULLET_DAMAGE + (otherBonus->type == Bonus::Damage ? BONUS_FORCE_DAMAGE_BOOST : 0));
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
        const std::optional<Position> &pos = r.get_entity_component<Position>(me);
        const int bon = rand() % 10;
        if (pos) {
            if (bon == 0)
                BonusHealth::create(r, pos.value());
            if (bon == 1)
                BonusForce::create(r, pos.value());
            if (bon == 2)
                BonusTripleShot::create(r, pos.value());
        }
    }

    Network::INetworkServer &network = Server::getInstance().getNetwork();
    for (auto &&[network_id] : Zipper(r.get_components<NetworkId>()))
        network.send(network_id.id, packet.serialize());
}

entity_t EnemyFly::create(Registry &r) {
    const entity_t entity = r.spawn_entity();

    Position pos(WIDTH, static_cast<float>(std::rand() % (HEIGHT - FLY_SIZE - FLY_ZIGZAG_SIZE)));

    r.add_component(entity, ComponentEntityType(Protocol::ENEMY_FLY));
    r.add_component(entity, Position(pos));
    r.add_component(entity, Velocity(FLY_SPEED_X + CAMERA_SPEED, FLY_SPEED_Y));
    r.add_component(entity, Life(FLY_HEALTH, FLY_HEALTH));
    r.add_component(entity, Collision(FLY_SIZE, FLY_SIZE, collision));
    r.add_component(entity, ::ArtificialIntelligence(ArtificialIntelligence()));

    Network::Packet packet(
        Protocol::SpawnEntityPacket(
            entity,
            Protocol::ENEMY_FLY,
            Protocol::Vector2f(pos.x, pos.y),
            Protocol::Vector2f(FLY_SIZE, FLY_SIZE),
            Protocol::Vector2f(FLY_SPEED_X + CAMERA_SPEED, FLY_SPEED_Y),
            FLY_HEALTH
        ),
        Protocol::SPAWN
    );
    Network::INetworkServer &network = Server::getInstance().getNetwork();
    for (auto &&[network_id] : Zipper(r.get_components<NetworkId>()))
        network.send(network_id.id, packet.serialize());
    return entity;
}
