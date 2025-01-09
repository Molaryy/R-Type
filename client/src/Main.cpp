/*
** EPITECH PROJECT, 2024
** R_Type
** File description:
** Main
*/

#include "Main.hpp"
#include "ArgHandler.hpp"
#include "Client.hpp"

int main(const int ac, char const *const *av) {
    try {
        if (ac == 2 && ArgHandler::check_help(ac, av[0], av[1]))
            return EXIT_SUCCESS;
        ArgHandler args(0);

        args.verif_arg(ac, std::vector<std::string_view>(av, av + ac));

        Client::createInstance(args.ip, args.port).run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_ERROR;
    }
    return EXIT_SUCCESS;
}
