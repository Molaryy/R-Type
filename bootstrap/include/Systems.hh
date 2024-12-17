/*
** EPITECH PROJECT, 2024
** Visual Studio Live Share (Espace de travail)
** File description:
** Systems
*/

#pragma once

#include "Registry.hh"
#include "Components.hh"

#include "raylib.hh"

//! la gestion de renderer doit être celle de la raylib, pareil pour input
class Systems
{
    public:
        Systems() = default;
        ~Systems() = default;
        void MovementSystem(registry &reg, float t)
        {
            auto &positions = reg.get_components<Position_t>();
            auto &velocities = reg.get_components<Velocity_t>();

            for (std::size_t i = 0; i < positions.size(); ++i)
            {
                if (positions[i].has_value() && velocities[i].has_value())
                {
                    auto &position = positions[i].value();
                    auto &velocity = velocities[i].value();

                    position.x += velocity.x * t;
                    position.y += velocity.y * t;
                }
            }
        }

        void CollisionSystem(registry &reg)
        {
            auto &position = reg.get_components<Position_t>();
            auto &collider = reg.get_components<Collider_t>();

            for (std::size_t i = 0; i < position.size(); ++i)
            {
                if (!position[i].has_value() || !collider[i].has_value())
                    continue;
                collider[i]->is_colliding = false;

                for (std::size_t j = i + 1; j < collider.size(); ++j)
                {
                    if (!position[j].has_value() || !collider[j].has_value())
                        continue;
                    if (collider[i]->x + collider[i]->width > collider[j]->x &&
                        collider[i]->x < collider[j]->x + collider[j]->width &&
                        collider[i]->y + collider[i]->height > collider[j]->y &&
                        collider[i]->y < collider[j]->y + collider[j]->height)
                    {
                        collider[i]->is_colliding = true;
                        collider[j]->is_colliding = true;
                    }
                }
            }
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
            rtype::RayLib &raylib;

            auto &positions = reg.get_components<Position_t>();
            auto &drawables = reg.get_components<Drawable_t>();

            raylib.BeginDrawing();
            raylib.ClearBackground(BLACK);

            for (std::size_t i = 0; i < positions.size(); ++i)
            {
                if (positions[i].has_value() && drawables[i].has_value())
                {
                    auto &position = positions[i].value();
                    auto &drawable = drawables[i].value();

                    Texture2D texture = raylib.LoadTexture(drawable.path.c_str());
                    raylib.DrawTexture(texture, position.x, position.y, WHITE);
                    raylib.UnloadTexture(texture);
                }
            }
            raylib.EndDrawing();
        }

        void PlayerInputSystem(registry &reg, Input &input)
        {
            auto &controllable = reg.get_components<Controllable_t>();
            auto &velocity = reg.get_components<Velocity_t>();

            for (std::size_t i = 0; i < controllable.size(); ++i)
            {
                if (controllable[i].has_value() && velocity[i].has_value())
                {
                    velocity[i]->x = 0;
                    velocity[i]->y = 0;

                    if (input.is_key_pressed("UP"))
                        velocity[i]->y = -controllable[i]->speed;
                    if (input.is_key_pressed("DOWN"))
                        velocity[i]->y = controllable[i]->speed;
                    if (input.is_key_pressed("LEFT"))
                        velocity[i]->x = -controllable[i]->speed;
                    if (input.is_key_pressed("RIGHT"))
                        velocity[i]->x = controllable[i]->speed;
                }
            }
        }

        void ProjectileSystem(registry &reg, float t)
        {
            auto &projectile = reg.get_components<Projectile_t>();
            auto &positions = reg.get_components<Position_t>();

            for (std::size_t i = 0; i < projectile.size(); ++i)
            {
                if (positions[i].has_value() && projectile[i].has_value())
                {
                    positions[i]->x += projectile[i]->speed * t;
                    projectile[i]->lifetime -= t;

                    if (projectile[i]->lifetime <= 0)
                        reg.remove_component<Projectile_t>(i);
                }
            }
        }
};
