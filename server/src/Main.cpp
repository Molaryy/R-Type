/*
** EPITECH PROJECT, 2024
** B-CPP-500-PAR-5-2-bsrtype-arthaud.poupard
** File description:
** main.cpp
*/

#include <iostream>

#include "../includes/ArgHandler.hpp"
#include "../includes/Server.hpp"

int main(const int ac, char const *const *av) {
    try {
        if (ac == 2 && ArgHandler::check_help(ac, av[0], av[1]))
            return EXIT_SUCCESS;
        ArgHandler args(7);

        args.verif_arg(ac, std::vector<std::string_view>(av, av + ac));

        Server::createInstance(args.port, args.max_lobby, args.max_client, args.debug).run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
}
