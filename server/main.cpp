/*
** EPITECH PROJECT, 2024
** B-CPP-500-PAR-5-2-bsrtype-arthaud.poupard
** File description:
** main.cpp
*/

#include <iostream>

#include "Arguments.hpp"
#include "Server.hpp"

int main(const int argc, char *const argv[]) {
    try {
        Arguments args;
        args.getArguments(argc, argv);

        Server::createInstance(args.getPort(), args.getNbClients()).run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
}
