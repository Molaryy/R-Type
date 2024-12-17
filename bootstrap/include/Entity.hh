/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** entity
*/

#pragma once

#include <cstddef> // std::size_t

#include "SparseArray.hh"

using entity_id = std::size_t;
struct entity_t {
    entity_id id;
    sparse_array<> components;
};
