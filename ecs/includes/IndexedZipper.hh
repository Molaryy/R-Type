/*
** EPITECH PROJECT, 2024
** ecs
** File description:
** IndexedZipper.hpp
*/

#pragma once

#include <algorithm>

#include "IndexedZipperIterator.hh"

template<class... Containers>
class IndexedZipper {
public :
    using iterator = IndexedZipperIterator<Containers...>;
    using iterator_tuple = typename iterator::iterator_tuple;
    explicit IndexedZipper(Containers &... cs)
        : begin_(std::make_tuple(cs.begin()...)),
          end_(std::make_tuple(cs.end()...)),
          size_((std::min)({static_cast<std::size_t>(cs.size())...})) {
    }

    iterator begin() {
        return iterator(begin_, end_, size_);
    }

    iterator end() {
        auto zipper_iterator = iterator(end_, end_, size_);
        zipper_iterator.index_ = size_;
        return zipper_iterator;
    }

private :
    iterator_tuple begin_;
    iterator_tuple end_;
    std::size_t size_;
};
