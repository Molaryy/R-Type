#pragma once

#include <unordered_map> // std::unordered_map
#include <typeindex>     // std::type_index
#include <any>           // std::any
#include <stdexcept>     // std::runtime_error

#include "Entity.hh"

class registry
{
public:
    /**
     * @brief Construct a new registry object
     *
     * @tparam Component
     * @return sparse_array<Component> &
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
     * @return sparse_array<Component> &
     */
    template <class Component>
    sparse_array<Component> &get_components()
    {
        auto type = std::type_index(typeid(Component));
        if (components_arrays_.find(type) == components_arrays_.end())
        {
            throw std::runtime_error("Component not registered");
        }
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
        if (components_arrays_.find(type) == components_arrays_.end())
        {
            throw std::runtime_error("Component not registered");
        }
        return std::any_cast<sparse_array<Component> const &>(components_arrays_.at(type));
    }

    /**
     * @brief Spawn a new entity
     *
     * @return entity_t
     * @return entity_t
     */
    entity_t spawn_entity()
    {
        entity_t new_id = next_entity_id_++;
        entities_container_[new_id] = new_id;
        return new_id;
    }

    /**
     * @brief Get the entity from id object
     *
     * @param index Entity id
     * @return entity_t
     */
    entity_t entity_from_id(std::size_t index)
    {
        return entities_container_[index];
    }

    /**
     * @brief Kill an entity
     *
     * @param id Entity id
     * @return void
     */
    void kill_entity(entity_id const &id)
    {
        entities_container_.erase(id);
    }

    /**
     * @brief Add a component to an entity
     *
     * @tparam Component
     * @param entity Entity id
     * @param component Component to add
     * @return typename sparse_array<Component>::reference_type
     */
    template <typename Component>
    typename sparse_array<Component>::reference_type add_component(entity_t const &entity, Component &&component)
    {
        return get_components<Component>().insert_at(entity, std::forward<Component>(component));
    }

    /**
     * @brief Get the component object
     *
     * @tparam Component
     * @param entity Entity id
     * @return typename sparse_array<Component>::reference_type
     */
    template <typename Component, typename... Params>
    typename sparse_array<Component>::reference_type emplace_component(entity_t const &entity, Params &&...params)
    {
        return get_components<Component>().emplace_at(entity, std::forward<Params>(params)...);
    }

    /**
     * @brief Get the component object
     *
     * @tparam Component
     * @param entity Entity id
     * @return typename sparse_array<Component>::reference_type
     */
    template <typename Component>
    void remove_component(entity_t const &entity)
    {
        get_components<Component>().erase(entity);
    }

private:
    entity_id next_entity_id_ = 0;
    std::unordered_map<entity_id, entity_t> entities_container_;
    std::unordered_map<std::type_index, std::any> components_arrays_;
};