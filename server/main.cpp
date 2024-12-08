/*
** EPITECH PROJECT, 2024
** B-CPP-500-PAR-5-2-bsrtype-arthaud.poupard
** File description:
** main.cpp
*/

#include <iostream>

#include "Server.hpp"
#include "Arguments.hpp"

int main(int argc, char const *const *argv)
{
    Arguments args;

    args.getArguments(argc, argv);

    try
    {
        asio::io_context io_context;
        Server my_server(io_context, args.getPort(), args.getNbClients());
        my_server.run();
    }
    catch (Arguments::ArgumentsException &e)
    {
        std::cerr << e.what() << std::endl;
        return 84;
    }
}
