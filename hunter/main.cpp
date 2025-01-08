/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main.cpp
*/

#include "Hunter.hpp"

int main() {
    try {
        Hunter::createInstance().run();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 84;
    }
}
