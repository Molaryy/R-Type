/*
** EPITECH PROJECT, 2025
** R_Type
** File description:
** Game scene
*/

#include "Scenes.hpp"
#include "Gameplay.hpp"

void newGameCallback(Registry &r)
{
    r.clear_enities();
    entity_t player = r.spawn_entity();
    r.add_component(player, Components::Entity(0, 0, 50, 50));
    r.add_component(player, Components::Player(100, "Player", 0));
    r.add_component(player, Components::Drawable(Textures::PLAYER_ID));
    r.add_component(player, Components::Movable(1));
    // add collidable component
}
