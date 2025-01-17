/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** CollisionSystem.cpp
*/

#include "Platformer.hpp"
#include "Zipper.hh"
#include "IndexedZipper.hh"

void Platform::collisionSystem(Registry &r) {
    auto &positions = r.get_components<Position>();
    auto &velocities = r.get_components<Velocity>();
    auto &collisions = r.get_components<Collision>();
    auto &players = r.get_components<PlayerTag>();
    auto &platforms = r.get_components<PlatformTag>();
    auto &springs = r.get_components<SpringTag>();
    auto &breakables  = r.get_components<BreakableTag>();

    float dt = getInstance().dt_;
    std::vector<entity_t> entitiesToKill;

    for (auto &&[pPos, pVel, pCol, player] : Zipper(positions, velocities, collisions, players)) {
        float left = pPos.x;
        float right = pPos.x + pCol.width;
        float newBottom = pPos.y + pCol.height;
        float oldBottom = newBottom - pVel.y * dt;

        for (auto &&[pos, vel, col, platform] : Zipper(positions, velocities, collisions, platforms)) {
            float platLeft = pos.x;
            float platRight = pos.x + col.width;
            float platTop = pos.y;

            bool overLapX = (right > platLeft && left < platRight);
            bool crossingTop = (oldBottom <= platTop && newBottom >= platTop);

            if (overLapX && crossingTop) {
                pPos.y = platTop - pCol.height;
                pVel.y = 0.f;
            }
        }
        {
            float minDelta = std::numeric_limits<float>::max();
            size_t targetEntity = 0;
            bool found = false;
            float targetBTop = 0.f;

            for (auto &&[entity, bPos, bCol, breakable] : IndexedZipper(positions, collisions, breakables)) {
                if (breakable.broken)
                    continue;
                float bLeft = bPos.x;
                float bRight = bPos.x + bCol.width;
                float bTop = bPos.y;

                bool overlapX = {right > bLeft && left < bRight};
                bool crossingTop = (oldBottom <= bTop && newBottom >= bTop);

                if (overlapX && crossingTop) {
                    float deltaY = bTop - oldBottom;
                    if (deltaY < minDelta) {
                        minDelta = deltaY;
                        targetEntity = entity;
                        targetBTop = bTop;
                        found = true;
                    }
                }
            }
            if (found && minDelta < 20.f) {
                pPos.y = targetBTop - pCol.height;
                pVel.y = -800.f;
                entitiesToKill.push_back(targetEntity);
                continue;
            }
        }
        for (auto &&[sprPos, sprCol, spring] : Zipper(positions, collisions, springs)) {
            float sprLeft  = sprPos.x;
            float sprRight = sprPos.x + sprCol.width;
            float sprTop   = sprPos.y;

            bool overlapX = (right > sprLeft && left < sprRight);
            bool crossingTop = (oldBottom <= sprTop && newBottom >= sprTop);

            if (overlapX && crossingTop) {
                pPos.y = sprTop - pCol.height;
                pVel.y = -1000.f;
            }
        }
    }
    for (auto &entity : entitiesToKill) {
        r.kill_entity(entity);
    }
}
