/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** registry
*/

#pragma once

#include <unordered_map> // std::unordered_map
#include <typeindex> // std::type_index
#include <any> // std::any

#include "entity_manager.hh"

class registry
{
    public:
        template <class Component>
        sparse_array<Component> &register_component()
        {
            auto type = std::type_index(typeid(Component));
            components_arrays_[type] = sparse_array<Component>();
            return std::any_cast<sparse_array<Component> &>(components_arrays_[type]);
        }

        template <class Component>
        sparse_array<Component> &get_components()
        {
            auto type = std::type_index(typeid(Component));
            return std::any_cast<sparse_array<Component> &>(components_arrays_.at(type));
        }

        template <class Component>
        sparse_array<Component> const &get_components() const
        {
            auto type = std::type_index(typeid(Component));
            return std::any_cast<sparse_array<Component> const &>(components_arrays_.at(type));
        }

        entity_t spawn_entity()
        {
            return entity_manager_.spawn_entity();
        }

        entity_t entity_from_index(std::size_t index)
        {
            return entity_manager_.entity_from_index(index);
        }

        void kill_entity(entity_t const &entity)
        {
            entity_manager_.kill_entity(entity);
        }

        template <typename Component>
        typename sparse_array<Component>::reference_type add_component(entity_t const &entity, Component &&component)
        {
            return get_components<Component>().insert_at(entity, std::forward<Component>(component));
        }

        template <typename Component, typename... Params>
        typename sparse_array<Component>::reference_type emplace_component(entity_t const &entity, Params &&...params)
        {
            return get_components<Component>().emplace_at(entity, std::forward<Params>(params)...);
        }

        template <typename Component>
        void remove_component(entity_t const &entity)
        {
            get_components<Component>().erase(entity);
        }

    private:
        std::unordered_map<std::type_index, std::any> components_arrays_;
        entity_manager entity_manager_;
};
