/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Systems
*/

#pragma once


#include <queue>
#include <thread>

#include "Components.hh"
#include "IndexedZipper.hh"
#include "Registry.hh"
#include "Zipper.hh"

namespace Systems {
    [[maybe_unused]] static void position_velocity(Registry &r) {
        SparseArray<Position> &positions = r.get_components<Position>();
        const SparseArray<Velocity> &velocitys = r.get_components<Velocity>();

        for (const auto &&[pos, vel] : Zipper(positions, velocitys)) {
            pos.x += vel.x;
            pos.y += vel.y;
        }
    }

    [[maybe_unused]] static void log(const Registry &r) {
        for (entity_t i = 0; i < r.max_entities(); ++i) {
            std::cout << "Entity " << i << ": ";
            r.log(i);
            std::cout << std::endl;
            if (i == r.max_entities() - 1)
                std::cout << std::endl;
        }
    }

    [[maybe_unused]] static void limit_framerate([[maybe_unused]] Registry &r, const uint8_t fps) {
        static const auto frame_duration = std::chrono::microseconds(1 / fps);
        static auto last_frame = std::chrono::steady_clock::now();
        const auto current_frame = std::chrono::steady_clock::now();
        const auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_frame - last_frame);

        if (elapsed_time < frame_duration)
            std::this_thread::sleep_for(frame_duration - elapsed_time);
        last_frame = std::chrono::steady_clock::now();
    }

    [[maybe_unused]] static void generic_collide(Registry &r) {
        std::queue<std::function<void(Registry &)>> collisionCallbacks;
        auto &positions = r.get_components<Position>();
        auto &collisions = r.get_components<Collision>();

        IndexedZipper zipper(positions, collisions);

        for (auto it1 = zipper.begin(); it1 != zipper.end(); ++it1) {
            for (auto it2 = std::next(it1); it2 != zipper.end(); ++it2) {
                auto &&[e1, pos1, col1] = *it1;
                auto &&[e2, pos2, col2] = *it2;

                if (pos1.x < pos2.x + col2.width &&
                    pos1.x + col1.width > pos2.x &&
                    pos1.y < pos2.y + col2.height &&
                    pos1.y + col1.height > pos2.y) {
                    col1.is_colliding = true;
                    col2.is_colliding = true;

                    collisionCallbacks.emplace([&col1, e1, e2](Registry &registry) {
                        col1.collisionTask(registry, e1, e2);
                    });
                    collisionCallbacks.emplace([&col2, e2, e1](Registry &registry) {
                        col2.collisionTask(registry, e2, e1);
                    });
                }
            }
        }
        while (!collisionCallbacks.empty()) {
            std::function<void(Registry &)> function = collisionCallbacks.front();
            function(r);
            collisionCallbacks.pop();
        }
    }
}
