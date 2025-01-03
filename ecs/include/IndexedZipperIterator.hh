/*
** EPITECH PROJECT, 2024
** ecs
** File description:
** IndexedZipperIterator.hpp
*/

#pragma once

#include <tuple>

template<class... Containers>
class IndexedZipper;

template<class... Containers>

class IndexedZipperIterator {
public:
    friend IndexedZipper<Containers...>;

    template<class Container>
    using iterator_t = decltype(std::declval<Container>().begin());
    template<class Container>
    using it_reference_t = typename iterator_t<Container>::reference;
    using value_type = std::tuple<std::size_t, decltype(std::declval<it_reference_t<Containers>>().value()) &...>;
    using reference = value_type &;
    using pointer = value_type *;
    using difference_type = std::size_t;
    using iterator_category = std::input_iterator_tag;
    using iterator_tuple = std::tuple<iterator_t<Containers>...>;

    IndexedZipperIterator(iterator_tuple const &it_tuple, iterator_tuple const &end_tuple, const std::size_t max)
        : _current(it_tuple), _end(end_tuple), _max(max), _idx(0) {
        if (_idx < _max && !all_set(_seq))
            incr_all(_seq);
    }

    IndexedZipperIterator(IndexedZipperIterator const &z) = default;
    IndexedZipperIterator operator++() {
        incr_all(_seq);
        return *this;
    }

    IndexedZipperIterator operator++(int) {
        IndexedZipperIterator res(*this);
        ++*this;
        return res;
    }

    value_type operator*() {
        return to_value(_seq);
    }

    value_type operator->() {
        return to_value(_seq);
    }

    friend bool operator==(IndexedZipperIterator const &lhs, IndexedZipperIterator const &rhs) {
        return lhs._idx == rhs._idx;
    }

    friend bool operator!=(IndexedZipperIterator const &lhs, IndexedZipperIterator const &rhs) {
        return lhs._idx != rhs._idx;
    }

private:
    template<std::size_t... Is>
    void incr_all(std::index_sequence<Is...>) {
        do {
            (std::get<Is>(_current)++, ...);
            _idx++;
        } while (_idx < _max && !all_set(_seq));
    }

    template<std::size_t... Is>
    bool all_set(std::index_sequence<Is...>) {
        return ((std::get<Is>(_current) != std::get<Is>(_end) && (*std::get<Is>(_current)).has_value()) && ...);
    }

    template<std::size_t... Is>
    value_type to_value(std::index_sequence<Is...>) {
        return std::tie(_idx, (*std::get<Is>(_current)).value()...);
    }

    iterator_tuple _current;
    iterator_tuple _end;
    std::size_t _max;
    std::size_t _idx;

    static constexpr std::index_sequence_for<Containers...> _seq{};
};
