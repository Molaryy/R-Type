/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Systems
*/

#pragma once

#include "Registry.hh"
#include "Components.hh"

#include "../../client/raylib/raylib.hh"

//! la gestion de renderer doit être celle de la raylib, pareil pour input
//! need a zipper to iterate over multiple components instead of multiple loops
class Systems
{
public:
    Systems() = default;
    ~Systems() = default;
    void MovementSystem(registry &reg, float t)
    {
        auto &positions = reg.get_components<Position_t>();
        auto &velocities = reg.get_components<Velocity_t>();

        // for (std::size_t i = 0; i < positions.size(); ++i)
        // {
        // if (positions[i].has_value() && velocities[i].has_value())
        // {
        // auto &position = std::any_cast<Position_t &>(positions[i]);
        // auto &velocity = std::any_cast<Velocity_t &>(velocities[i]);
        //
        // position.x += velocity.x * t;
        // position.y += velocity.y * t;
        // }
        // }
    }

    void CollisionSystem(registry &reg)
    {
        auto &position = reg.get_components<Position_t>();
        auto &collider = reg.get_components<Collision_t>();

        // for (std::size_t i = 0; i < position.size(); ++i)
        // {
        // if (!position[i].has_value() || !collider[i].has_value())
        // continue;
        // auto &colliderComponent = std::any_cast<Collision_t &>(collider[i]);
        // colliderComponent.is_colliding = false;
        //
        // for (std::size_t j = i + 1; j < collider.size(); ++j)
        // {
        // if (!position[j].has_value() || !collider[j].has_value())
        // continue;
        // auto &colliderComponent2 = std::any_cast<Collision_t &>(collider[j]);
        //
        // if (colliderComponent.x < colliderComponent2.x + colliderComponent2.width &&
        // colliderComponent.x + colliderComponent.width > colliderComponent2.x &&
        // colliderComponent.y < colliderComponent2.y + colliderComponent2.height &&
        // colliderComponent.y + colliderComponent.height > colliderComponent2.y)
        // {
        // colliderComponent.is_colliding = true;
        // colliderComponent2.is_colliding = true;
        // }
        // }
        // }
    }

    // void RenderSystem(registry &reg, Renderer &renderer)
    // {
    //     auto &position = reg.get_components<Position_t>();
    //     auto &sprite = reg.get_components<Sprite_t>();

    //     for (std::size_t i = 0; i < position.size(); ++i)
    //     {
    //         if (position[i].has_value() && sprite[i].has_value())
    //         {
    //             renderer.draw(sprite[i]->texture, position[i]->x, position[i]->y);
    //         }
    //     }
    // }

    //! using raylib
    void RenderSystem(registry &reg)
    {
        rtype::RayLib raylib;

        auto &positions = reg.get_components<Position_t>();
        auto &drawables = reg.get_components<Drawable_t>();

        raylib.beginDrawing();
        raylib.clearBackground(BLACK);

        // for (std::size_t i = 0; i < positions.size(); ++i)
        // {
        // if (positions[i].has_value() && drawables[i].has_value())
        // {
        // auto &position = std::any_cast<Position_t &>(positions[i]);
        // auto &drawable = std::any_cast<Drawable_t &>(drawables[i]);
        //
        // Texture2D texture = raylib.loadTexture(drawable.path.c_str());
        // raylib.drawTexture(texture, position.x, position.y, WHITE);
        // raylib.unloadTexture(texture);
        // }
        // }
        raylib.endDrawing();
    }

    // void PlayerInputSystem(registry &reg, Input &input)
    // {
    // auto &controllable = reg.get_components<Controllable_t>();
    // auto &velocity = reg.get_components<Velocity_t>();
    //
    // for (std::size_t i = 0; i < controllable.size(); ++i)
    // {
    // if (controllable[i].has_value() && velocity[i].has_value())
    // {
    // auto &vel = std::any_cast<Velocity_t &>(velocity[i]);
    // vel.x = 0;
    // vel.y = 0;
    //
    // if (input.is_key_pressed("UP"))
    // vel.y = -std::any_cast<Controllable_t &>(controllable[i]).speed;
    // if (input.is_key_pressed("DOWN"))
    // vel.y = std::any_cast<Controllable_t &>(controllable[i]).speed;
    // if (input.is_key_pressed("LEFT"))
    // vel.x = -std::any_cast<Controllable_t &>(controllable[i]).speed;
    // if (input.is_key_pressed("RIGHT"))
    // vel.x = std::any_cast<Controllable_t &>(controllable[i]).speed;
    // }
    // }
    // }

    void ProjectileSystem(registry &reg, float t)
    {
        auto &projectile = reg.get_components<Projectile_t>();
        auto &positions = reg.get_components<Position_t>();

        // for (std::size_t i = 0; i < projectile.size(); ++i)
        // {
        // if (positions[i].has_value() && projectile[i].has_value())
        // {
        // auto &position = std::any_cast<Position_t &>(positions[i]);
        // auto &proj = std::any_cast<Projectile_t &>(projectile[i]);
        //
        // position.x += proj.speed * t;
        // proj.lifetime -= t;
        //
        // if (proj.lifetime <= 0)
        // reg.kill_entity(i);
        // }
        // }
    }
};
