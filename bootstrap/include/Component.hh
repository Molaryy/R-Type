/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** component
*/

#pragma once

#include <vector> // std::vector
#include <memory> // std::allocator_traits
#include <any> //std::any
#include <algorithm> // std::find

template <class Allocator = std::allocator<std::any>>
class sparse_array
{
    public:
        using value_type = std::any;
        using reference_type = value_type &;
        using const_reference_type = value_type const &;
        using container_t = std::vector<value_type, Allocator>;
        using size_type = typename container_t::size_type;
        using iterator = typename container_t::iterator;
        using const_iterator = typename container_t::const_iterator;

        sparse_array() noexcept = default;
        sparse_array(sparse_array const &) noexcept = default;
        sparse_array(sparse_array &&) noexcept = default;
        ~sparse_array() noexcept = default;
        sparse_array &operator=(sparse_array const &) noexcept = default;
        sparse_array &operator=(sparse_array &&) noexcept = default;

        reference_type operator[](size_t index) { return data_[index]; }
        const_reference_type operator[](size_t index) const { return data_[index]; }

        iterator begin() noexcept { return data_.begin(); }
        const_iterator begin() const noexcept { return data_.begin(); }
        const_iterator cbegin() const noexcept { return data_.cbegin(); }
        iterator end() noexcept { return data_.end(); }
        const_iterator end() const noexcept { return data_.end(); }
        const_iterator cend() const noexcept { return data_.cend(); }
        size_type size() const noexcept { return data_.size(); }

        template <typename Component>
        reference_type insert_at(size_type pos, Component const &value);
        template <typename Component>
        reference_type insert_at(size_type pos, Component &&value);
        template <class... Params>
        reference_type emplace_at(size_type pos, Params &&...params);
        void erase(size_type pos);
        size_type get_index(value_type const &value) const;

    private:
        container_t data_;
};
