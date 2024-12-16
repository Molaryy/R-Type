/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Registry
*/

#include "Registry.hh"

template <class Component>
sparse_array<Component> &registry::register_component()
{
    auto type = std::type_index(typeid(Component));
    components_arrays_[type] = sparse_array<Component>();
    return std::any_cast<sparse_array<Component> &>(components_arrays_[type]);
}

template <class Component>
sparse_array<Component> &registry::get_components()
{
    auto type = std::type_index(typeid(Component));
    return std::any_cast<sparse_array<Component> &>(components_arrays_.at(type));
}

template <class Component>
sparse_array<Component> const &registry::get_components() const
{
    auto type = std::type_index(typeid(Component));
    return std::any_cast<sparse_array<Component> const &>(components_arrays_.at(type));
}

entity_t registry::spawn_entity()
{
    entities_container_[next_entity_id_++] = entity_t{ };
    return entities_container_.at(next_entity_id_ - 1);
}

entity_t registry::entity_from_id(std::size_t index)
{
    return entities_container_.at(index);
}

void registry::kill_entity(entity_id const &id)
{
    entities_container_.erase(id);
}

template <typename Component>
typename sparse_array<Component>::reference_type registry::add_component(entity_t const &entity, Component &&component)
{
    return get_components<Component>().insert_at(entity, std::forward<Component>(component));
}

template <typename Component, typename... Params>
typename sparse_array<Component>::reference_type registry::emplace_component(entity_t const &entity, Params &&...params)
{
    return get_components<Component>().emplace_at(entity, std::forward<Params>(params)...);
}

template <typename Component>
void registry::remove_component(entity_t const &entity)
{
    get_components<Component>().erase(entity);
}
