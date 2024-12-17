/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main.cpp
*/

// #include "Client.hpp"
// #include "../../server/includes/Game.hpp"
#include <iostream>
#include "Menu/Menu.hpp"
#include "../bootstrap/include/SparseArray.hh"

void print_any(const std::any& a)
{
    if (a.type() == typeid(int))
    {
        std::cout << std::any_cast<int>(a) << std::endl;
    }
    else if (a.type() == typeid(double))
    {
        std::cout << std::any_cast<double>(a) << std::endl;
    }
    else if (a.type() == typeid(std::string))
    {
        std::cout << std::any_cast<std::string>(a) << std::endl;
    }
    else
    {
        std::cout << "Type non supporté" << std::endl;
    }
}

void print_sparse_array(const sparse_array<> &entities)
{
    for (size_t i = 0; i < entities.size(); ++i)
    {
        if (entities[i].has_value())
        {
            std::cout << "id: " << i << " value: ";
            print_any(entities[i]);
        }
    }
}

int main()
{
    sparse_array<> entities;
    rtype::RayLib rl;

    auto factory_entities = [](sparse_array<> &entities, int id, std::any value)
    {
        entities.insert_at(id, value);
    };

    factory_entities(entities, 0, 42);
    print_sparse_array(entities);
    print_any(entities[0]);

    Menu menu(rl);
    menu.run(rl);
}


// int main() {
//     try {
//         const std::string serverIP = "0.0.0.0";
//         const unsigned int port = 8081;

//         Client client(serverIP, port);
//         client.run();
//         Interaction interaction;
//         interaction.setInteraction(1, 0, 0, 0);
//         client.sendInteraction(interaction);
//     } catch (const std::exception &e) {
//         std::cerr << "Client error: " << e.what() << std::endl;
//     }
//     return 0;
// }
