/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** SparseArray
*/

#pragma once

#include <vector>    // std::vector
#include <memory>    // std::allocator
#include <any>       // std::any
#include <algorithm> // std::find
#include <optional>  //std::optional

//! Primary template any T
template <typename T = std::any, class Allocator = std::allocator<std::optional<T>>>
class sparse_array
{
public:
    using value_type = std::optional<T>;
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

    reference_type operator[](size_type index) noexcept { return data_[index]; }
    const_reference_type operator[](size_type index) const noexcept { return data_[index]; }

    iterator begin() noexcept { return data_.begin(); }
    const_iterator begin() const noexcept { return data_.begin(); }
    const_iterator cbegin() const noexcept { return data_.cbegin(); }

    iterator end() noexcept { return data_.end(); }
    const_iterator end() const noexcept { return data_.end(); }
    const_iterator cend() const noexcept { return data_.cend(); }

    size_type size() const noexcept { return data_.size(); }

    /**
     * @brief Insert a value at the end of the array
     *
     * @param pos Position to insert the value
     * @param value Value to insert
     * @return reference_type
     */
    reference_type insert_at(size_type pos, value_type const &value)
    {
        if (pos >= data_.size())
            data_.resize(pos + 1);
        data_[pos] = value;
        return data_[pos];
    }

    /**
     * @brief Insert a value at the end of the array
     *
     * @param pos Position to insert the value
     * @param value Value to insert
     * @return reference_type
     */
    reference_type insert_at(size_type pos, value_type &&value)
    {
        if (pos >= data_.size())
            data_.resize(pos + 1);
        data_[pos] = std::move(value);
        return data_[pos];
    }

    /**
     * @brief Insert a value at the end of the array
     *
     * @tparam Params Parameters to forward to the value constructor
     * @param pos Position to insert the value
     * @param params Parameters to forward to the value constructor
     * @return reference_type
     */
    template <class... Params>
    reference_type emplace_at(size_type pos, Params &&...params)
    {
        if (pos >= data_.size())
            data_.resize(pos + 1);
        data_[pos] = value_type(std::forward<Params>(params)...);
        return data_[pos];
    }

    /**
     * @brief Erase a value at a specific position
     *
     * @param pos Position to erase
     * @return void
     */
    void erase(size_type pos)
    {
        if (pos < data_.size())
            data_.erase(data_.begin() + pos);
    }

    /**
     * @brief Get the index object
     *
     * @param value Value to search
     * @return size_type
     */
    size_type get_index(value_type const &value) const noexcept
    {
        auto iter = std::find(data_.begin(), data_.end(), value);
        if (iter == data_.end())
            return data_.size();
        return static_cast<size_type>(std::distance(data_.begin(), iter));
    }

    /**
     * @brief Get the at object
     *
     * @param pos Position to get the value
     * @return value_type &
     */
    value_type &get_at(size_t pos) { return data_.at(pos); }

    /**
     * @brief Get the at object
     *
     * @param pos Position to get the value
     * @return value_type const &
     */
    value_type const &get_at(size_t pos) const { return data_.at(pos); }

private:
    container_t data_;
};

//! Partial specialization for std::any
template <>
class sparse_array<std::any, std::allocator<std::any>>
{
public:
    using value_type = std::any;
    using reference_type = value_type &;
    using const_reference_type = value_type const &;
    using container_t = std::vector<value_type>;
    using size_type = typename container_t::size_type;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    sparse_array() noexcept = default;
    sparse_array(sparse_array const &) noexcept = default;
    sparse_array(sparse_array &&) noexcept = default;
    ~sparse_array() noexcept = default;

    sparse_array &operator=(sparse_array const &) noexcept = default;
    sparse_array &operator=(sparse_array &&) noexcept = default;

    reference_type operator[](size_type index) noexcept { return data_[index]; }
    const_reference_type operator[](size_type index) const noexcept { return data_[index]; }

    iterator begin() noexcept { return data_.begin(); }
    const_iterator begin() const noexcept { return data_.begin(); }
    const_iterator cbegin() const noexcept { return data_.cbegin(); }

    iterator end() noexcept { return data_.end(); }
    const_iterator end() const noexcept { return data_.end(); }
    const_iterator cend() const noexcept { return data_.cend(); }

    size_type size() const noexcept { return data_.size(); }

    /**
     * @brief Insert a value at the end of the array
     *
     * @param pos Position to insert the value
     * @param value Value to insert
     * @return reference_type
     */
    reference_type insert_at(size_type pos, value_type const &value)
    {
        if (pos >= data_.size())
            data_.resize(pos + 1);
        data_[pos] = value;
        return data_[pos];
    }

    /**
     * @brief Insert a value at the end of the array
     *
     * @param pos Position to insert the value
     * @param value Value to insert
     * @return reference_type
     */
    reference_type insert_at(size_type pos, value_type &&value)
    {
        if (pos >= data_.size())
            data_.resize(pos + 1);
        data_[pos] = std::move(value);
        return data_[pos];
    }

    /**
     * @brief Insert a value at the end of the array
     *
     * @tparam Params Parameters to forward to the value constructor
     * @param pos Position to insert the value
     * @param params Parameters to forward to the value constructor
     * @return reference_type
     */
    template <class... Params>
    reference_type emplace_at(size_type pos, Params &&...params)
    {
        if (pos >= data_.size())
            data_.resize(pos + 1);
        data_[pos] = value_type(std::forward<Params>(params)...);
        return data_[pos];
    }

    /**
     * @brief Erase a value at a specific position
     *
     * @param pos Position to erase
     * @return void
     */
    void erase(size_type pos)
    {
        if (pos < data_.size())
            data_.erase(data_.begin() + pos);
    }

    /**
     * @brief Get the index object
     *
     * @param value Value to search
     * @return size_type
     */
    size_type get_index(value_type const &value) const noexcept
    {
        auto iter = std::find_if(data_.begin(), data_.end(), [&value](value_type const &val)
                                 { return value.type() == val.type(); });
        if (iter == data_.end())
            return data_.size();
        return static_cast<size_type>(std::distance(data_.begin(), iter));
    }

    /**
     * @brief Get the at object
     *
     * @param pos Position to get the value
     * @return value_type &
     */
    value_type &get_at(size_t pos) { return data_.at(pos); }

    /**
     * @brief Get the at object
     *
     * @param pos Position to get the value
     * @return value_type const &
     */
    value_type const &get_at(size_t pos) const { return data_.at(pos); }

private:
    container_t data_;
};
