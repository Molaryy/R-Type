/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Component
*/

#include "Component.hh"

template class sparse_array<std::allocator<std::any>>;
template <class Allocator>
template <typename Component>
typename sparse_array<Allocator>::reference_type sparse_array<Allocator>::insert_at(size_type pos, Component const &value)
{
    if (pos >= data_.size())
        data_.resize(pos + 1);
    else
        data_[pos].reset();

    data_[pos] = value;
    return data_[pos];
}

template <class Allocator>
template <typename Component>
typename sparse_array<Allocator>::reference_type sparse_array<Allocator>::insert_at(size_type pos, Component &&value)
{
    if (pos >= data_.size())
        data_.resize(pos + 1);
    else
        data_[pos].reset();

    data_[pos] = std::move(value);
    return data_[pos];
}


template <class Allocator>
template <class... Params>
typename sparse_array<Allocator>::reference_type sparse_array<Allocator>::emplace_at(size_type pos, Params &&...params)
{
    if (pos >= data_.size())
        data_.resize(pos + 1);
    else
        data_[pos].reset();

    data_[pos] = std::any(std::forward<Params>(params)...);
    return data_[pos];
}

template <class Allocator>
void sparse_array<Allocator>::erase(size_type pos)
{
    if (pos < data_.size())
    {
        data_[pos].reset();
        data_.erase(data_.begin() + pos);
    }
}

template <class Allocator>
typename sparse_array<Allocator>::size_type sparse_array<Allocator>::get_index(value_type const &value) const
{
    auto iter = std::find_if(data_.begin(), data_.end(), [&value](const std::any& elem)
    {
        return elem.has_value() && elem.type() == value.type() && std::any_cast<const void*>(&elem) == std::any_cast<const void*>(&value);
    });

    if (iter != data_.end())
        return iter - data_.begin();
    return data_.size();
}
