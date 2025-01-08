/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Zipper
*/

#pragma once

#include <tuple>    //std::tuple
#include <iterator> //std::forward_iterator_tag

template <class... Containers>
class Zipper;

template <class... Containers>
class ZipperIterator {
    friend Zipper<Containers...>;

    template <class Container>
    using iterator_t = typename Container::iterator;

    template <class Container>
    using it_reference_t = typename iterator_t<Container>::reference;

public:
    using value_type = std::tuple<decltype(std::declval<it_reference_t<Containers>>().value()) &...>;
    using reference = value_type;
    using pointer = void;
    using difference_type = std::size_t;
    using iterator_category = std::forward_iterator_tag;
    using iterator_tuple = std::tuple<iterator_t<Containers>...>;

    ZipperIterator(iterator_tuple const &it_tuple, iterator_tuple const &end_tuple, const std::size_t max)
        : current_(it_tuple), end_(end_tuple), max_(max), index_(0) {
        if (index_ < max_ && !all_set(seq_))
            incr_all(seq_);
    }

    ZipperIterator(ZipperIterator const &z) = default;

    ZipperIterator operator++() {
        incr_all(seq_);
        return *this;
    }

    ZipperIterator operator++(int) {
        ZipperIterator temp = *this;
        ++(*this);
        return temp;
    }

    value_type operator*() {
        return to_value(seq_);
    }

    value_type operator->() {
        return to_value(seq_);
    }

    friend bool operator==(ZipperIterator const &lhs, ZipperIterator const &rhs) {
        return lhs.index_ == rhs.index_;
    }

    friend bool operator!=(ZipperIterator const &lhs, ZipperIterator const &rhs) {
        return lhs.index_ != rhs.index_;
    }

private:
    template <std::size_t... Is>
    void incr_all(std::index_sequence<Is...>) {
        do {
            (..., std::get<Is>(current_)++);
            index_++;
        } while (index_ < max_ && !all_set(seq_));
    }

    template <std::size_t... Is>
    bool all_set(std::index_sequence<Is...>) {
        return ((std::get<Is>(current_) != std::get<Is>(end_) && (*std::get<Is>(current_)).has_value()) && ...);
    }

    template <std::size_t... Is>
    value_type to_value(std::index_sequence<Is...>) {
        return std::tie((*std::get<Is>(current_)).value()...);
    }

    iterator_tuple current_;
    iterator_tuple end_;
    std::size_t max_;
    std::size_t index_;

    static constexpr std::index_sequence_for<Containers...> seq_{};
};
