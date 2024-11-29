/*
** EPITECH PROJECT, 2024
** B-CPP-500-PAR-5-2-bsrtype-arthaud.poupard
** File description:
** main.cpp
*/

#include <iostream>
#include "Server.hpp"
#include "ErrorHandling.hpp"

int main(int argc, char **argv) {
    ErrorHandling err;
    std::vector<int> error = err.errorHandling(argc, argv);

    if (error[0] == 84)
        return 84;
    try {
        server::Server my_server(error[1], error[2]);
        return my_server.run();
    } catch (std::invalid_argument &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
}
