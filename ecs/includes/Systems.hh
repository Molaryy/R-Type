/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Systems
*/

#pragma once

#include "Registry.hh"
#include "Components.hh"

#include <thread>

#define DEFAULT_FPS 33333 // 30 fps

namespace Systems
{
	static void log(const Registry &r)
    {
        for (entity_t i = 0; i < r.max_entities(); ++i)
        {
            std::cout << "Entity " << i << ": ";
            r.log(i);
            std::cout << std::endl;
            if (i == r.max_entities() - 1)
                std::cout << std::endl;
        }
    }

    static void limit_framerate(Registry &r)
    {
        static const auto frame_duration = std::chrono::microseconds(DEFAULT_FPS);
        static auto last_frame = std::chrono::steady_clock::now();
        const auto current_frame = std::chrono::steady_clock::now();
        const auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_frame - last_frame);

        if (elapsed_time < frame_duration)
            std::this_thread::sleep_for(frame_duration - elapsed_time);
        last_frame = std::chrono::steady_clock::now();
    }
}
