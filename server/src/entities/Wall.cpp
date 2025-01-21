/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Wall.cpp
*/

#include "entities/Wall.hpp"

#include "Components.hh"
#include "Components.hpp"
#include "Packet.hpp"
#include "Server.hpp"
#include "Zipper.hh"

void Wall::collision(Registry &r, const entity_t me, const entity_t other) {
    std::optional<Life> &otherLife = r.get_entity_component<Life>(other);
    const std::optional<Life> &life = r.get_entity_component<Life>(me);
    const std::optional<ComponentEntityType> &type = r.get_entity_component<ComponentEntityType>(other);
    if (!life.has_value() || !life->is_alive() || !otherLife.has_value() || !otherLife->is_alive() || !type.has_value() || type->type == Protocol::WALL
        || type->type == Protocol::BOSS)
        return;

    otherLife->current = 0;
}

entity_t Wall::create(Registry &r) {
    const Position pos(WIDTH, static_cast<float>(std::rand() % (HEIGHT - WALL_SIZE)));

    return createFromPos(r, pos);
}

entity_t Wall::createFromPos(Registry &r, const Position &position) {
    Position pos(position);

    const entity_t entity = r.spawn_entity();

    r.add_component(entity, ComponentEntityType(Protocol::WALL));
    r.add_component(entity, Position(pos));
    r.add_component(entity, Velocity(CAMERA_SPEED, 0));
    r.add_component(entity, Life(1, 1));
    r.add_component(entity, Collision(WALL_SIZE, WALL_SIZE, collision));

    Network::Packet packet(
        Protocol::SpawnEntityPacket(
            entity,
            Protocol::WALL,
            Protocol::Vector2f(pos.x, pos.y),
            Protocol::Vector2f(WALL_SIZE, WALL_SIZE),
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
