/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** entity
*/

#pragma once

#include <cstddef>

class entity
{
    public:
        explicit entity(size_t value) : value_(value) {}
        operator size_t() const { return value_; }
    private:
        size_t value_;
};
