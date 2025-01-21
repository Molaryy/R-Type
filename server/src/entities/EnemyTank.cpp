/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** EnemyTank.cpp
*/

#include "entities/EnemyTank.hpp"

#include "entities/BonusTripleShot.hpp"
#include "entities/BonusForce.hpp"
#include "entities/BonusHealth.hpp"
#include "Components.hh"
#include "Components.hpp"
#include "Packet.hpp"
#include "Server.hpp"
#include "Zipper.hh"
#include "entities/PlayerBullet.hpp"

void EnemyTank::collision(Registry &r, const entity_t me, const entity_t other) {
    const std::optional<ComponentEntityType> &otherType = r.get_entity_component<ComponentEntityType>(other);
    const std::optional<Bonus> &otherBonus = r.get_entity_component<Bonus>(other);
    std::optional<Life> &life = r.get_entity_component<Life>(me);
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
            if (Server::random(TANK_DROP_BONUS_HEALTH_CHANCE))
                BonusHealth::create(r, pos.value());
            else if (Server::random(TANK_DROP_BONUS_DAMAGE_CHANCE))
                BonusForce::create(r, pos.value());
            else if (Server::random(TANK_DROP_BONUS_TRIPLE_CHANCE))
                BonusTripleShot::create(r, pos.value());
        }
    }
}

entity_t EnemyTank::create(Registry &r) {
    const Position pos(WIDTH, static_cast<float>(std::rand() % (HEIGHT - TANK_SIZE)));

    return createFromPos(r, pos);
}

entity_t EnemyTank::createFromPos(Registry &r, const Position &position) {
    Position pos(position);

    const entity_t entity = r.spawn_entity();

    r.add_component(entity, ComponentEntityType(Protocol::ENEMY_FLY));
    r.add_component(entity, Position(pos));
    r.add_component(entity, Velocity(TANK_SPEED + CAMERA_SPEED, 0));
    r.add_component(entity, Life(TANK_HEALTH, TANK_HEALTH));
    r.add_component(entity, Collision(TANK_SIZE, TANK_SIZE, collision));

    Network::Packet packet(
        Protocol::SpawnEntityPacket(
            entity,
            Protocol::ENEMY_FLY,
            Protocol::Vector2f(pos.x, pos.y),
            Protocol::Vector2f(TANK_SIZE, TANK_SIZE),
            Protocol::Vector2f(TANK_SPEED + CAMERA_SPEED, 0),
            TANK_HEALTH
        ),
        Protocol::SPAWN
    );
    Network::INetworkServer &network = Server::getInstance().getNetwork();
    for (auto &&[network_id] : Zipper(r.get_components<NetworkId>()))
        network.send(network_id.id, packet.serialize());
    return entity;
}
