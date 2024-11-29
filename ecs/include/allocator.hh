/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** allocator
*/

#pragma once

template <typename T>
class custom_allocator
{
    public:
        using value_type = T;

        custom_allocator() = default;

        template <typename U>
        custom_allocator(const custom_allocator<U> &) noexcept { }

        T *allocate(std::size_t n)
        {
            return static_cast<T *>(::operator new(n * sizeof(T)));
        }

        void deallocate(T *p, std::size_t)
        {
            ::operator delete(p);
        }

        template <typename U>
        struct rebind
        {
            using other = custom_allocator<U>;
        };
};
