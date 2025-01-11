/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Systems
*/

#pragma once

#include <thread>

#include "Components.hh"
#include "Registry.hh"
#include "Zipper.hh"

namespace Systems {
    [[maybe_unused]] static void position_velocity(Registry &r) {
        auto &positions = r.get_components<Position>();
        auto &velocitys = r.get_components<Velocity>();

        for (auto &&[pos, vel] : Zipper(positions, velocitys)) {
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
}
