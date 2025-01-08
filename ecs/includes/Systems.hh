/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Systems
*/

#pragma once

#include "Registry.hh"
#include "Components.hh"

namespace Systems {
	static void log(const Registry &r) {
        for (entity_t i = 0; i < r.max_entities(); ++i) {
            std::cout << "Entity " << i << ": ";
            r.log(i);
            std::cout << std::endl;
            if (i == r.max_entities() - 1)
                std::cout << std::endl;
        }
    }
}
