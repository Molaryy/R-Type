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
class zipper_iterator
{
    template <class Container>
    using iterator_t = typename Container::iterator;

    template <class Container>
    using it_reference_t = typename iterator_t<Container>::reference;

public:
    using value_type = std::tuple<it_reference_t<Containers>...>;
    using reference = value_type;
    using pointer = void;
    using difference_type = std::size_t;
    using iterator_category = std::forward_iterator_tag;
    using iterator_tuple = std::tuple<iterator_t<Containers>...>;
    // friend containers::zipper<Containers...>; ??
    zipper_iterator(iterator_tuple const &iter_tuple, std::size_t max);

    zipper_iterator(zipper_iterator const &z);

    zipper_iterator operator++();
    zipper_iterator operator++(int)
    {
        zipper_iterator temp = *this;
        ++(*this);
        return temp;
    }

    value_type operator*() { return to_value(seq_); }
    value_type operator->() { return to_value(seq_); }

    friend bool operator==<>(zipper_iterator const &lhs, zipper_iterator const &rhs);
    friend bool operator!=<>(zipper_iterator const &lhs, zipper_iterator const &rhs);

private:
    // extract from tuple and increment each element 💀
    template <std::size_t... Is>
    void incr_all(std::index_sequence<Is...>) { (..., (++std::get<Is>(current_))); }

    template <std::size_t... Is>
    bool all_set(std::index_sequence<Is...>) { return (... && (std::get<Is>(current_)->has_value())); }

    // create a tuple from the current iterators
    template <std::size_t... Is>
    value_type to_value(std::index_sequence<Is...>) { return std::tie(*std::get<Is>(current_)...); }

    iterator_tuple current_;
    std::size_t max_;
    std::size_t index_;

    static constexpr std::index_sequence_for<Containers...> seq_{};
};

#include "Zipper.hxx"
