/*
** EPITECH PROJECT, 2024
** R-Type
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
    Texture2D GetTexture(const std::string &path)
    {
        if (textures_.find(path) == textures_.end())
            textures_[path] = rtype::RayLib().loadTexture(path.c_str());
        return textures_[path];
    }

    ~AssetManager()
    {
        for (auto &pair : textures_)
            rtype::RayLib().unloadTexture(pair.second);
    }

private:
    std::unordered_map<std::string, Texture2D> textures_;
};
