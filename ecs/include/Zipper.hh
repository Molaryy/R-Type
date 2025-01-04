/*
** EPITECH PROJECT, 2024
** ecs
** File description:
** Zipper.hpp
*/

#pragma once

#include <algorithm>

#include "ZipperIterator.hh"

template<class... Containers>
class Zipper {
public :
    using iterator = ZipperIterator<Containers...>;
    using iterator_tuple = typename iterator::iterator_tuple;

    explicit Zipper(Containers &... cs)
        : _begin(std::make_tuple(cs.begin()...)),
          _end(std::make_tuple(cs.end()...)),
          _size((std::min)({static_cast<std::size_t>(cs.size())...})) {
    }

    iterator begin() {
        return iterator(_begin, _end, _size);
    }

    iterator end() {
        auto zipper_iterator = iterator(_end, _end, _size);
        zipper_iterator._idx = _size;
        return zipper_iterator;
    }

private :
    iterator_tuple _begin;
    iterator_tuple _end;
    std::size_t _size;
};
