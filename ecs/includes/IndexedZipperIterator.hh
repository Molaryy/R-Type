/*
** EPITECH PROJECT, 2024
** ecs
** File description:
** IndexedZipperIterator.hpp
*/

#pragma once

#include <tuple>

template <class... Containers>
class IndexedZipper;

template <class... Containers>
class IndexedZipperIterator {
public:
    friend IndexedZipper<Containers...>;

    template <class Container>
    using iterator_t = decltype(std::declval<Container>().begin());
    template <class Container>
    using it_reference_t = typename iterator_t<Container>::reference;
    using value_type = std::tuple<std::size_t, decltype(std::declval<it_reference_t<Containers>>().value()) &...>;
    using reference = value_type &;
    using pointer = value_type *;
    using difference_type = std::size_t;
    using iterator_category = std::input_iterator_tag;
    using iterator_tuple = std::tuple<iterator_t<Containers>...>;

    IndexedZipperIterator(iterator_tuple const &it_tuple, iterator_tuple const &end_tuple, const std::size_t max)
        : current_(it_tuple), end_(end_tuple), max_(max) {
        index_ = end_tuple == it_tuple ? max : 0;
        if (index_ < max_ && !all_set(seq_))
            incr_all(seq_);
    }

    IndexedZipperIterator(IndexedZipperIterator const &z) = default;

    IndexedZipperIterator operator++() {
        incr_all(seq_);
        return *this;
    }

    IndexedZipperIterator operator++(int) {
        IndexedZipperIterator res(*this);
        ++*this;
        return res;
    }

    value_type operator*() {
        return to_value(seq_);
    }

    value_type operator->() {
        return to_value(seq_);
    }

    friend bool operator==(IndexedZipperIterator const &lhs, IndexedZipperIterator const &rhs) {
        return lhs.index_ == rhs.index_;
    }

    friend bool operator!=(IndexedZipperIterator const &lhs, IndexedZipperIterator const &rhs) {
        return lhs.index_ != rhs.index_;
    }

private:
    template <std::size_t... Is>
    void incr_all(std::index_sequence<Is...>) {
        if (index_ >= max_)
            return;
        do {
            (std::get<Is>(current_)++, ...);
            index_++;
        } while (index_ < max_ && !all_set(seq_));
    }

    template <std::size_t... Is>
    bool all_set(std::index_sequence<Is...>) {
        return ((std::get<Is>(current_) != std::get<Is>(end_) && (*std::get<Is>(current_)).has_value()) && ...);
    }

    template <std::size_t... Is>
    value_type to_value(std::index_sequence<Is...>) {
        return std::tie(index_, (*std::get<Is>(current_)).value()...);
    }

    iterator_tuple current_;
    iterator_tuple end_;
    std::size_t max_;
    std::size_t index_;

    static constexpr std::index_sequence_for<Containers...> seq_{};
};
