/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** SparseArray
*/

#pragma once

#include <vector> // std::vector
#include <memory> // std::allocator_traits
#include <any> //std::any

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

    public:
        /**
         * @brief Construct a new sparse array object
         *
         */
        sparse_array() noexcept = default;

        /**
         * @brief Construct a new sparse array object
         *
         */
        sparse_array(sparse_array const &) noexcept = default;

        /**
         * @brief Construct a new sparse array object
         *
         */
        sparse_array(sparse_array &&) noexcept = default;

        /**
         * @brief Destroy the sparse array object
         *
         */
        ~sparse_array() noexcept = default;

        /**
         * @brief Copy assignment operator
         *
         * @return sparse_array&
         */
        sparse_array &operator=(sparse_array const &) noexcept = default;

        /**
         * @brief Move assignment operator
         *
         * @return sparse_array&
         */
        sparse_array &operator=(sparse_array &&) noexcept = default;

        /**
         * @brief Get the value at the index
         *
         * @param index
         * @return reference_type
         */
        reference_type operator[](size_t index) { return data_[index]; }

        /**
         * @brief Get the value at the index
         *
         * @param index
         * @return const_reference_type
         */
        const_reference_type operator[](size_t index) const { return data_[index]; }

        /**
         * @brief Get the begin of the sparse array
         *
         * @return iterator
         */
        iterator begin() noexcept { return data_.begin(); }

        /**
         * @brief Get the begin of the sparse array
         *
         * @return const_iterator
         */
        const_iterator begin() const noexcept { return data_.begin(); }

        /**
         * @brief Get the begin of the sparse array
         *
         * @return const_iterator
         */
        const_iterator cbegin() const noexcept { return data_.cbegin(); }

        /**
         * @brief Get the end of the sparse array
         *
         * @return iterator
         */
        iterator end() noexcept { return data_.end(); }

        /**
         * @brief Get the end of the sparse array
         *
         * @return const_iterator
         */
        const_iterator end() const noexcept { return data_.end(); }

        /**
         * @brief Get the end of the sparse array
         *
         * @return const_iterator
         */
        const_iterator cend() const noexcept { return data_.cend(); }

        /**
         * @brief Get the size of the sparse array
         *
         * @return size_type
         */
        size_type size() const noexcept { return data_.size(); }


        // templated to be able to make any component type instead of just value_type
        /**
         * @brief Insert a value in the sparse array
         *
         * @tparam Component
         * @param pos
         * @param value
         * @return reference_type
         */
        template <typename Component>
        reference_type insert_at(size_type pos, Component const &value)
        {
            if (pos >= data_.size())
                data_.resize(pos + 1);
            else
                data_[pos].reset();

            data_[pos] = value;
            return data_[pos];
        }

        /**
         * @brief Insert a value in the sparse array
         *
         * @tparam Component
         * @param pos
         * @param value
         * @return reference_type
         */
        template <typename Component>
        reference_type insert_at(size_type pos, Component &&value)
        {
            if (pos >= data_.size())
                data_.resize(pos + 1);
            else
                data_[pos].reset();

            data_[pos] = std::forward<Component>(value);
            return data_[pos];
        }

        /**
         * @brief Emplace a value in the sparse array
         *
         * @tparam Params
         * @param pos
         * @param params
         * @return reference_type
         */
        template <class... Params>
        reference_type emplace_at(size_type pos, Params &&...params)
        {
            if (pos >= data_.size())
                data_.resize(pos + 1);
            else
                data_[pos].reset();

            data_[pos] = std::any(std::forward<Params>(params)...);
            return data_[pos];
        }

        /**
         * @brief Erase a value in the sparse array
         *
         * @param pos
         */
        void erase(size_type pos)
        {
            if (pos < data_.size())
            {
                data_[pos].reset();
                data_.erase(data_.begin() + pos);
            }
        }

        /**
         * @brief Get the index object
         *
         * @param value
         * @return size_type
         */
        size_type get_index(value_type const &value) const
        {
            auto iter = std::find(data_.begin(), data_.end(), value);

            if (iter != data_.end())
                return iter - data_.begin();
            return data_.size();
        }

        /**
         * @brief Get the at object
         * 
         * @tparam Component 
         * @param index 
         * @return Component& 
         */
        template <typename Component>
        Component &get_at(size_t index)
        {
            return std::any_cast<Component&>(data_[index]);
        }

        /**
         * @brief Get the at object
         * 
         * @tparam Component 
         * @param index 
         * @return const Component& 
         */
        template <typename Component>
        const Component &get_at(size_t index) const
        {
            return std::any_cast<const Component&>(data_[index]);
        }

    private:
        container_t data_;
};
