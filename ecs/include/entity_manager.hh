/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** entity_manager
*/

#pragma once

#include "entity.hh"
#include "component.hh"

using entity_t = entity;

class entity_manager
{
    public:
        entity_t spawn_entity()
        {
            entities_.push_back(entity_t{entity_counter_});
            return entity_t{entity_counter_++};
        }

        /**
         * @brief 
         * 
         * @param index 
         * @return entity_t 
         */
        entity_t entity_from_index(std::size_t index)
        {
            return entities_.at(index);
        }

        void kill_entity(entity_t const &entity)
        {
            entities_.erase(std::remove(entities_.begin(), entities_.end(), entity), entities_.end());
        }

        template <typename Component>
        typename sparse_array<Component>::reference_type add_component(entity_t const &entity, Component &&component)
        {
            return registry_.get_components<Component>().insert_at(entity, std::forward<Component>(component));
        }

        template <typename Component, typename... Params>
        typename sparse_array<Component>::reference_type emplace_component(entity_t const &entity, Params &&...params)
        {
            return registry_.get_components<Component>().emplace_at(entity, std::forward<Params>(params)...);
        }

        template <typename Component>
        void remove_component(entity_t const &entity)
        {
            registry_.get_components<Component>().erase(entity);
        }

    private:
        std::vector<entity_t> entities_;
        std::size_t entity_counter_ = 0;
};