/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Registry
*/

#pragma once

#include <unordered_map> // std::unordered_map
#include <typeindex> // std::type_index
#include <any> // std::any

#include "Entity.hh"
#include "Component.hh"

class registry
{
    public:
        template <class Component>
        sparse_array<Component> &register_component();
        template <class Component>
        sparse_array<Component> &get_components();
        template <class Component>
        sparse_array<Component> const &get_components() const;
        
        entity_t spawn_entity();
        entity_t entity_from_id(std::size_t index);
        void kill_entity(entity_id const &id);

        template <typename Component>
        typename sparse_array<Component>::reference_type add_component(entity_t const &entity, Component &&component);
        template <typename Component, typename... Params>
        typename sparse_array<Component>::reference_type emplace_component(entity_t const &entity, Params &&...params);
        template <typename Component>
        void remove_component(entity_t const &entity);

    private:
        entity_id next_entity_id_ = 0;
        std::unordered_map<entity_id, entity_t> entities_container_;
        std::unordered_map<std::type_index, std::any> components_arrays_;
};
