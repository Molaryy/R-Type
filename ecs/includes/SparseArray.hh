/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** SparseArray
*/

#pragma once

#include <algorithm>
#include <optional>
#include <vector>

template <typename Component>
class SparseArray {
public:
    using value_type = std::optional<Component>;
    using reference_type = value_type &;
    using const_reference_type = value_type const &;
    using container_t = std::vector<value_type>;
    using size_type = typename container_t::size_type;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    SparseArray() noexcept = default;
    SparseArray(SparseArray const &) noexcept = default;
    SparseArray(SparseArray &&) noexcept = default;
    ~SparseArray() noexcept = default;

    SparseArray &operator=(SparseArray const &) noexcept = default;
    SparseArray &operator=(SparseArray &&) noexcept = default;

    reference_type operator[](size_type index) noexcept {
        if (index >= size())
            resize(index + 1);
        return data_[index];
    }

    const_reference_type operator[](size_type index) const noexcept {
        if (index >= size())
            return null_;
        return data_[index];
    }

    iterator begin() noexcept {
        return data_.begin();
    }

    const_iterator begin() const noexcept {
        return data_.begin();
    }

    const_iterator cbegin() const noexcept {
        return data_.cbegin();
    }

    iterator end() noexcept {
        return data_.end();
    }

    const_iterator end() const noexcept {
        return data_.end();
    }

    const_iterator cend() const noexcept {
        return data_.cend();
    }

    size_type size() const noexcept {
        return data_.size();
    }

    void resize(size_type new_size) {
        data_.resize(new_size, std::nullopt);
    }

    /**
     * @brief Insert a value
     *
     * @param pos Position to insert the value
     * @param value Value to insert
     * @return reference_type
     */
    reference_type insert_at(size_type pos, value_type const &value) {
        if (pos >= data_.size())
            data_.resize(pos + 1);
        data_[pos] = value;
        return data_[pos];
    }

    /**
     * @brief Insert a value
     *
     * @param pos Position to insert the value
     * @param value Value to insert
     * @return reference_type
     */
    reference_type insert_at(size_type pos, value_type &&value) {
        if (pos >= data_.size())
            data_.resize(pos + 1);
        data_[pos] = std::move(value);
        return data_[pos];
    }

    /**
     * @brief Emplace a value
     *
     * @tparam Params Parameters to forward to the value constructor
     * @param pos Position to insert the value
     * @param params Parameters to forward to the value constructor
     * @return reference_type
     */
    template <class... Params>
    reference_type emplace_at(size_type pos, Params &&... params) {
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
    void erase(size_type pos) {
        if (pos < data_.size())
            data_[pos].reset();
    }

    /**
     * @brief Get the index object
     *
     * @param value Value to search
     * @return size_type
     */
    size_type get_index(value_type const &value) const noexcept {
        auto iter = std::ranges::find(data_, value);
        if (iter == data_.end())
            return data_.size();
        return static_cast<size_type>(std::distance(data_.begin(), iter));
    }

private:
    container_t data_;

    value_type null_ = std::nullopt;
};
