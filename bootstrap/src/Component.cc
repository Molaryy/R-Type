/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Component
*/

#include "Component.hh"

template class sparse_array<std::allocator<std::any>>;
template typename sparse_array<std::allocator<std::any>>::reference_type sparse_array<std::allocator<std::any>>::insert_at<int>(size_type, int &&);
template typename sparse_array<std::allocator<std::any>>::reference_type sparse_array<std::allocator<std::any>>::insert_at<int>(size_type, int const &);
template typename sparse_array<std::allocator<std::any>>::reference_type sparse_array<std::allocator<std::any>>::emplace_at<int>(size_type, int &&);