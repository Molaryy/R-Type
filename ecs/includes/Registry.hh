/*
** EPITECH PROJECT, 2024
** ecs
** File description:
** registry.hpp
*/

#pragma once

#include <any>
#include <functional>
#include <iostream>
#include <ranges>
#include <stdexcept>
#include <typeindex>
#include <unordered_map>

#include "Entity.hh"
#include "SparseArray.hh"


class Registry {
public:
    template <class Component>
    SparseArray<Component> &register_component() {
        const std::type_index idx(typeid(Component));
        if (!components_arrays_.contains(idx)) {
            components_arrays_.emplace(idx, SparseArray<Component>());
        }
        if (!remove_functions_.contains(idx)) {
            remove_functions_.emplace(idx, [](Registry &reg, entity_t const &ent) {
                auto &array = reg.get_components<Component>();
                array.erase(ent);
            });
        }
        if (!loggers_.contains(idx)) {
            loggers_.emplace(idx, [](Registry const &reg, entity_t const &ent) {
                const auto &array = reg.get_components<Component>();
                const std::optional<Component> &comp = array[ent];
                if (comp.has_value())
                    comp.value().log();
                return comp.has_value();
            });
        }
        return std::any_cast<SparseArray<Component> &>(components_arrays_[idx]);
    }


    template <class Component>
    SparseArray<Component> &get_components() {
        const std::type_index idx(typeid(Component));
        if (!components_arrays_.contains(idx))
            register_component<Component>();
        std::any &components = components_arrays_[idx];
        return std::any_cast<SparseArray<Component> &>(components);
    }

    template <class Component>
    SparseArray<Component> const &get_components() const {
        const std::type_index idx(typeid(Component));
        if (!components_arrays_.contains(idx))
            throw std::runtime_error("Component array not registered");
        const std::any &components = components_arrays_.find(idx)->second;
        return std::any_cast<const SparseArray<Component> &>(components);
    }

    [[nodiscard]] entity_t spawn_entity() {
        if (!dead_entities_.empty()) {
            const entity_t entity = dead_entities_.back();
            dead_entities_.pop_back();
            return entity;
        }
        next_entity_ += 1;
        return next_entity_ - 1;
    }

    entity_t entity_from_index(const std::size_t idx) {
        if (std::ranges::find(dead_entities_, idx) != dead_entities_.end() || idx >= next_entity_)
            throw std::runtime_error("Entity index out of range.");
        return idx;
    }

    void kill_entity(entity_t const &e) {
        dead_entities_.push_back(e);
        for (const std::function<void(Registry &, const entity_t &)> &remove_function : std::views::values(remove_functions_))
            remove_function(*this, e);
    }

    void clear_enities() {
        next_entity_ = 0;
        remove_functions_.clear();
        dead_entities_.clear();
        loggers_.clear();
        components_arrays_.clear();
    }

    template <typename Component>
    typename SparseArray<Component>::reference_type add_component(entity_t const &to, Component &&c) {
        if (!components_arrays_.contains(typeid(Component)))
            register_component<Component>();
        return get_components<Component>().insert_at(to, c);
    }

    template <typename Component, typename... Params>
    typename SparseArray<Component>::reference_type emplace_component(entity_t const &to, Params &&... p) {
        if (!components_arrays_.contains(typeid(Component)))
            register_component<Component>();
        return get_components<Component>().emplace_at(to, std::forward<Params>(p)...);
    }

    template <typename Component>
    void remove_component(entity_t const &from) {
        const std::type_index idx(typeid(Component));
        if (!components_arrays_.contains(idx))
            register_component<Component>();
        if (remove_functions_.contains(idx))
            remove_functions_[idx](*this, from);
    }

    template <typename Function>
    void add_system(Function &&f) {
        systems_.push_back([f = std::forward<Function>(f)](Registry &reg) {
            f(reg);
        });
    }

    void remove_system(const std::size_t idx) {
        if (idx >= systems_.size())
            return;
        systems_.erase(systems_.begin() + static_cast<long>(idx));
    }

    void clear_systems() {
        systems_.clear();
    }

    void run_systems() {
        std::vector<std::function<void(Registry &)>> systems_copy = systems_;
        for (std::function<void(Registry &)> &system : systems_copy)
            system(*this);
    }

    void log(const entity_t &entity) const {
        for (const auto &logger : std::views::values(loggers_)) {
            if (logger(*this, entity))
                std::cout << ", ";
        }
    }

    std::size_t max_entities() const {
        return next_entity_;
    }

private:
    std::unordered_map<std::type_index, std::any> components_arrays_;

    std::unordered_map<std::type_index, std::function<void(Registry &, entity_t const &)>> remove_functions_;
    std::unordered_map<std::type_index, std::function<bool(const Registry &, const entity_t &)>> loggers_;
    std::vector<std::function<void(Registry &)>> systems_;

    std::vector<entity_t> dead_entities_;
    std::size_t next_entity_{0};
};
