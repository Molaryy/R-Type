/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** BonusForce.cpp
*/

#include "entities/BonusForce.hpp"

#include "Components.hh"
#include "Components.hpp"
#include "Packet.hpp"
#include "RTypeProtocol.hpp"
#include "Server.hpp"
#include "Zipper.hh"

void BonusForce::collision(Registry &r, const entity_t me, const entity_t other) {
    const std::optional<ComponentEntityType> otherType = r.get_components<ComponentEntityType>()[other];
    std::optional<Life> &life = r.get_components<Life>()[me];
    if (!otherType.has_value() || !life.has_value() || !life->is_alive() || otherType->type != Protocol::PLAYER)
        return;

    std::optional<Bonus> &bonus = r.get_components<Bonus>()[other];
    if (!bonus.has_value())
        return;
    if (bonus->type != Bonus::None) {
        r.kill_entity(bonus->id);
        Network::Packet packet(
            Protocol::DeadPacket(me, false),
            Protocol::KILL
        );
        Network::INetworkServer &network = Server::getInstance().getNetwork();
        for (auto &&[network_id] : Zipper(r.get_components<NetworkId>()))
            network.send(network_id.id, packet.serialize());
    }
    bonus->id = me;
    bonus->type = Bonus::Damage;
}

entity_t BonusForce::create(Registry &r, const Position position) {
    const entity_t entity = r.spawn_entity();

    r.add_component(entity, ComponentEntityType(Protocol::BONUS_DAMAGE));
    r.add_component(entity, Position(position));
    r.add_component(entity, Velocity(CAMERA_SPEED, 0));
    r.add_component(entity, Life(1, 1));
    r.add_component(entity, Collision(BONUS_FORCE_SIZE, BONUS_FORCE_SIZE, collision));

    Network::Packet packet(
        Protocol::SpawnEntityPacket(
            entity,
            Protocol::BONUS_DAMAGE,
            Protocol::Vector2f(position.x, position.y),
            Protocol::Vector2f(BONUS_FORCE_SIZE, BONUS_FORCE_SIZE),
            Protocol::Vector2f(CAMERA_SPEED, 0),
            1
        ),
        Protocol::SPAWN
    );
    Network::INetworkServer &network = Server::getInstance().getNetwork();
    for (auto &&[network_id] : Zipper(r.get_components<NetworkId>()))
        network.send(network_id.id, packet.serialize());
    return entity;
}
