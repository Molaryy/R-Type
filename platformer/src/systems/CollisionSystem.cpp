/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** CollisionSystem.cpp
*/

#include <limits>
#include <algorithm>
#include "Platformer.hpp"
#include "Zipper.hh"
#include "IndexedZipper.hh"

void Platform::collisionSystem(Registry &r) {
    SparseArray<Position> &positions = r.get_components<Position>();
    SparseArray<Velocity> &velocities = r.get_components<Velocity>();
    SparseArray<Collision> &collisions = r.get_components<Collision>();
    SparseArray<EntityType> &entityType = r.get_components<EntityType>();

    const float dt = getInstance().dt_;
    std::vector<entity_t> entitiesToKill;

    for (auto &&[pPos, pVel, pCol, player] : Zipper(positions, velocities, collisions, entityType)) {
        if (player.type == PlayerType) {
            const float left = pPos.x;
            const float right = pPos.x + pCol.width;
            const float newBottom = pPos.y + pCol.height;
            const float oldBottom = newBottom - pVel.y * dt;

            for (auto &&[pos, vel, col, platform] : Zipper(positions, velocities, collisions, entityType)) {
                if (platform.type == PlatformType) {
                    const float platLeft = pos.x;
                    const float platRight = pos.x + col.width;
                    const float platTop = pos.y;

                    const bool overLapX = right > platLeft && left < platRight;
                    const bool crossingTop = oldBottom <= platTop && newBottom >= platTop;

                    if (overLapX && crossingTop) {
                        pPos.y = platTop - pCol.height;
                        pVel.y = 0.f;
                    }
                }
            }
            {
                float minDelta = (std::numeric_limits<float>::max)();
                size_t targetEntity = 0;
                bool found = false;
                float targetBTop = 0.f;

                for (auto &&[entity, bPos, bCol, breakable] : IndexedZipper(positions, collisions, entityType)) {
                    if (breakable.type == BreakableType) {
                        if (breakable.broken)
                            continue;
                        const float bLeft = bPos.x;
                        const float bRight = bPos.x + bCol.width;
                        const float bTop = bPos.y;

                        const bool overlapX = right > bLeft && left < bRight;
                        const bool crossingTop = oldBottom <= bTop && newBottom >= bTop;

                        if (overlapX && crossingTop) {
                            const float deltaY = bTop - oldBottom;
                            if (deltaY < minDelta) {
                                minDelta = deltaY;
                                targetEntity = entity;
                                targetBTop = bTop;
                                found = true;
                            }
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
            for (auto &&[sprPos, sprCol, spring] : Zipper(positions, collisions, entityType)) {
                if (spring.type == SpringType) {
                    const float sprLeft  = sprPos.x;
                    const float sprRight = sprPos.x + sprCol.width;
                    const float sprTop   = sprPos.y;

                    const bool overlapX = right > sprLeft && left < sprRight;
                    const bool crossingTop = oldBottom <= sprTop && newBottom >= sprTop;

                    if (overlapX && crossingTop) {
                        pPos.y = sprTop - pCol.height;
                        pVel.y = -1000.f;
                    }
                }
            }
        }
    }
    for (entity_t &entity : entitiesToKill) {
        r.kill_entity(entity);
    }
}
