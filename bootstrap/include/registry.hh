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
        /**
         * @brief Construct a new registry object
         * 
         * @tparam Component 
         * @return sparse_array<Component>& 
         */
        template <class Component>
        sparse_array<Component> &register_component()
        {
            auto type = std::type_index(typeid(Component));
            components_arrays_[type] = sparse_array<Component>();
            return std::any_cast<sparse_array<Component> &>(components_arrays_[type]);
        }

        /**
         * @brief Get the components object
         * 
         * @tparam Component 
         * @return sparse_array<Component>& 
         */
        template <class Component>
        sparse_array<Component> &get_components()
        {
            auto type = std::type_index(typeid(Component));
            return std::any_cast<sparse_array<Component> &>(components_arrays_.at(type));
        }

        /**
         * @brief Get the components object
         * 
         * @tparam Component 
         * @return sparse_array<Component> const & 
         */
        template <class Component>
        sparse_array<Component> const &get_components() const
        {
            auto type = std::type_index(typeid(Component));
            return std::any_cast<sparse_array<Component> const &>(components_arrays_.at(type));
        }
        
        /**
         * @brief spawn an entity
         * 
         * @return entity_t 
         */
        entity_t spawn_entity()
        {
            entities_container_[next_entity_id_++] = entity_t{ };
            return entities_container_.at(next_entity_id_ - 1);
        }

        /**
         * @brief return the entity from the id
         * 
         * @param id 
         * @return entity_t 
         */
        entity_t entity_from_id(entity_id id)
        {
            return entities_container_.at(id);
        }

        /**
         * @brief kill an entity
         * 
         * @param entity 
         */
        void kill_entity(entity_t const &entity)
        {
            entities_container_.erase(
                std::remove(entities_container_.begin(),
                    entities_container_.end(),
                    entity
                ),
                entities_container_.end()
            );
        }

        /**
         * @brief add a component to an entity
         * 
         * @tparam Component 
         * @param entity 
         * @param component 
         * @return sparse_array<Component>::reference_type 
         */
        template <typename Component>
        typename sparse_array<Component>::reference_type add_component(entity_t const &entity, Component &&component)
        {
            return get_components<Component>().insert_at(entity, std::forward<Component>(component));
        }

        /**
         * @brief emplace a component to an entity
         * 
         * @tparam Component 
         * @tparam Params 
         * @param entity 
         * @param params 
         * @return sparse_array<Component>::reference_type 
         */
        template <typename Component, typename... Params>
        typename sparse_array<Component>::reference_type emplace_component(entity_t const &entity, Params &&...params)
        {
            return get_components<Component>().emplace_at(entity, std::forward<Params>(params)...);
        }

        /**
         * @brief remove a component from an entity
         * 
         * @tparam Component 
         * @param entity 
         */
        template <typename Component>
        void remove_component(entity_t const &entity)
        {
            get_components<Component>().erase(entity);
        }

    private:
        entity_id next_entity_id_ = 0;
        std::unordered_map<entity_id, entity_t> entities_container_;
};
