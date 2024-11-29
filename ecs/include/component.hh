/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** component
*/

#pragma once

#include <cstdlib> // free, malloc
#include <vector> // std::vector
#include <algorithm> // std::find
#include <memory> // std::allocator_traits
#include <utility> // std::forward
#include <any> //std::any

#include "allocator.hh"

template <typename Component, typename Allocator = std::allocator<Component>>
class sparse_array
{
    public:
        using value_type = std::any
        using reference_type = value_type &;
        using const_reference_type = value_type const &;
        using container_t = std::vector<value_type, Allocator>;
        using size_type = typename container_t::size_type;
        using iterator = typename container_t::iterator;
        using const_iterator = typename container_t::const_iterator;

    public:
        /// @brief  Default constructor
        sparse_array() = default;

        /// @brief  Copy constructor
        sparse_array(sparse_array const &) = default;

        /// @brief  Move constructor
        sparse_array(sparse_array &&) noexcept = default;
    
        /// @brief  Destructor
        ~sparse_array() = default;

        /// @brief  Copy assignment operator
        sparse_array &operator=(sparse_array const &) = default;

        /// @brief  Move assignment operator
        sparse_array &operator=(sparse_array &&) noexcept = default;

        /// @brief Get the value at the index
        /// @param index  The index of the value
        /// @return  reference_type
        reference_type operator[](size_t index) { return data_[index]; }

        /// @brief Get the value at the index
        /// @param index  The index of the value
        /// @return  const_reference_type
        const_reference_type operator[](size_t index) const { return data_[index]; }

        /// @brief Get the begin of the sparse array
        /// @return iterator
        iterator begin() noexcept { return data_.begin(); }

        /// @brief Get the begin of the sparse array
        /// @return const_iterator
        const_iterator begin() const noexcept { return data_.begin(); }

        /// @brief Get the begin of the sparse array
        /// @return const_iterator
        const_iterator cbegin() const noexcept { return data_.cbegin(); }

        /// @brief Get the end of the sparse array
        /// @return iterator
        iterator end() noexcept { return data_.end(); }

        /// @brief Get the end of the sparse array
        /// @return const_iterator
        const_iterator end() const noexcept { return data_.end(); }

        /// @brief Get the end of the sparse array
        /// @return const_iterator
        const_iterator cend() const noexcept { return data_.cend(); }

        /// @brief Get the size of the sparse array
        /// @return size_type
        size_type size() const noexcept { return data_.size(); }

        /// @brief Insert a value in the sparse array
        /// @param pos  The position to insert the value
        /// @param value  The value to insert
        /// @return  reference_type
        reference_type insert_at(size_type pos, Component const &value)
        {
            if (pos >= data_.size())
            {
                data_.resize(pos + 1);
            }
            else
            {
                std::allocator_traits<Allocator>::destroy(data_.get_allocator(), std::addressof(data_[pos]));
            }
            std::allocator_traits<Allocator>::construct(data_.get_allocator(), std::addressof(data_[pos]), value);
            return data_[pos];
        }

        /// @brief Insert a value in the sparse array
        /// @param pos  The position to insert the value
        /// @param value  The value to insert
        /// @return  reference_type
        reference_type insert_at(size_type pos, Component &&value)
        {
            if (pos >= data_.size())
            {
                data_.resize(pos + 1);
            }
            else
            {
                std::allocator_traits<Allocator>::destroy(data_.get_allocator(), std::addressof(data_[pos]));
            }
            std::allocator_traits<Allocator>::construct(data_.get_allocator(), std::addressof(data_[pos]), std::move(value));
            return data_[pos];
        }

        /// @brief Emplace a value in the sparse array
        /// @tparam ...Params  The parameters to pass to the constructor of the value
        /// @param pos The position to insert the value
        /// @param ...  The parameters to pass to the constructor of the value
        /// @return  reference_type
        template <class... Params>
        reference_type emplace_at(size_type pos, Params &&...params)
        {
            if (pos >= data_.size())
            {
                data_.resize(pos + 1);
            }
            else
            {
                std::allocator_traits<Allocator>::destroy(data_.get_allocator(), std::addressof(data_[pos]));
            }
            std::allocator_traits<Allocator>::construct(data_.get_allocator(), std::addressof(data_[pos]), std::forward<Params>(params)...);
            return data_[pos];
        }

        /// @brief Erase a value in the sparse array
        /// @param pos  The position of the value to erase
        void erase(size_type pos)
        {
            if (pos < data_.size())
            {
                std::allocator_traits<Allocator>::destroy(data_.get_allocator(), std::addressof(data_[pos]));
                data_.erase(data_.begin() + pos);
            }
        }

        /// @brief Get the index of a value in the sparse array
        /// @param value
        /// @return size_type
        size_type get_index(value_type const &value) const
        {
            auto iter = std::find(data_.begin(), data_.end(), value);

            if (iter != data_.end())
                return std::distance(data_.begin(), iter);
            return data_.size();
        }

    private:
        container_t data_;
};
