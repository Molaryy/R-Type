/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Zipper
*/

#pragma once

#include "Zipper.hh"

template <class... Containers>
zipper_iterator<Containers...>::zipper_iterator(iterator_tuple const &iter_tuple, std::size_t max)
    : current_{iter_tuple}, max_{max}, index_{0}
{
}

template <class... Containers>
zipper_iterator<Containers...>::zipper_iterator(zipper_iterator const &z)
    : current_{z.current_}, max_{z.max_}, index_{z.index_}
{
}

template <class... Containers>
zipper_iterator<Containers...> zipper_iterator<Containers...>::operator++()
{
    incr_all(seq_); //seq meaning those we wish to increment : 0, 1, 2, 3, 4, 5...(all)
    index_++;
    return *this;
}

template <class... Containers>
bool operator==(zipper_iterator<Containers...> const &lhs, zipper_iterator<Containers...> const &rhs)
{
    return (lhs.current_ == rhs.current_);
}

template <class... Containers>
bool operator!=(zipper_iterator<Containers...> const &lhs, zipper_iterator<Containers...> const &rhs)
{
    return !(lhs == rhs);
}
