/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Systems
*/

#pragma once

#include "Registry.hh"
#include "Components.hh"
#include "IndexedZipper.hh"

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

    static void limit_framerate([[maybe_unused]]Registry &r, const uint8_t fps)
    {
        static const auto frame_duration = std::chrono::microseconds(1 / fps);
        static auto last_frame = std::chrono::steady_clock::now();
        const auto current_frame = std::chrono::steady_clock::now();
        const auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_frame - last_frame);

        if (elapsed_time < frame_duration)
            std::this_thread::sleep_for(frame_duration - elapsed_time);
        last_frame = std::chrono::steady_clock::now();
    }

    static void generic_collide(Registry &r)
    {
        auto &positions = r.get_components<Position>();
        auto &collisions = r.get_components<Collision>();
        auto &relations = r.get_components<Relation>();

        IndexedZipper zipper(positions, collisions, relations);

        for (auto it1 = zipper.begin(); it1 != zipper.end(); ++it1)
        {
            for (auto it2 = std::next(it1); it2 != zipper.end(); ++it2)
            {
                const auto &pos1 = std::get<1>(*it1); //start to 1 because first is the entity id.
                auto &col1 = std::get<2>(*it1);
                const auto &rel1 = std::get<3>(*it1);

                const auto &pos2 = std::get<1>(*it2);
                auto &col2 = std::get<2>(*it2);
                const auto &rel2 = std::get<3>(*it2);

                if (rel1.is_ally != rel2.is_ally)
                {
                    if (col1.x < pos2.x + 1 &&
                        col1.x + 1 > pos2.x &&
                        col1.y < pos2.y + 1 &&
                        col1.y + 1 > pos2.y)
                    {
                        col1.is_colliding = true;
                        col2.is_colliding = true;
                    }
                }
            }
        }
    }
}
