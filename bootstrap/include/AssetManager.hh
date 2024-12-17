/*
** EPITECH PROJECT, 2024
** Visual Studio Live Share (Espace de travail)
** File description:
** AssetManager
*/

#pragma once

#include "raylib.hh"

#include <unordered_map>
#include <string>

class AssetManager
{
    public:

        rtype::RayLib::Texture2D GetTexture(const std::string &path)
        {
            if (textures_.find(path) == textures_.end())
                textures_[path] = rtype::RayLib::LoadTexture(path);
            return textures_[path];
        }

        ~AssetManager()
        {
            for (auto &pair : textures_)
                rtype::RayLib::UnloadTexture(pair.second);
        }
    private:
        std::unordered_map<std::string, rtype::RayLib::Texture2D> textures_;
};
