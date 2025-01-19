/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main.cpp
*/

#include "Platformer.hpp"

int main() {
    try {
        Platform::createInstance().run();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 84;
    }
}
