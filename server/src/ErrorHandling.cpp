/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ErrorHandling.cpp
*/

#include "ErrorHandling.hpp"

void ErrorHandling::help()
{
    std::cout << "USAGE: ./r-type_server [OPTIONS]" << std::endl;
    std::cout << "OPTIONS:" << std::endl;
    std::cout << "\t-h, --help\t\t\tDisplay this help message" << std::endl;
    std::cout << "\t-p, --port=PORT\t\t\tSet the port of the server" << std::endl;
}

std::vector<int> ErrorHandling::errorHandling(int argc, char **argv)
{
    int port = 0;
    int max_clients = 0;

    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "-h") == 0 || std::strcmp(argv[i], "--help") == 0) {
            help();
            exit(0);
        } else if (std::strcmp(argv[i], "-p") == 0 || std::strcmp(argv[i], "--port") == 0) {
            if (i + 1 < argc) {
                if (checkPort(argv[i + 1]) == 84) {
                    std::cerr << "Invalid port" << std::endl;
                    return {84, port, max_clients};
                }
                port = std::atoi(argv[i + 1]);
                ++i;
            } else {
                std::cerr << "Invalid port" << std::endl;
                return {84, port, max_clients};
            }
        } else if (std::strcmp(argv[i], "-c") == 0 || std::strcmp(argv[i], "--clients") == 0) {
            if (i + 1 < argc) {
                if (checkPort(argv[i + 1]) == 84) {
                    std::cerr << "Invalid number of clients" << std::endl;
                    return {84, port, max_clients};
                }
                max_clients = std::atoi(argv[i + 1]);
                ++i;
            } else {
                std::cerr << "Invalid number of clients" << std::endl;
                return {84, port, max_clients};
            }
        } else {
            std::cerr << "Invalid argument" << std::endl;
            return {84, port, max_clients};
        }
    }
    return {0, port, max_clients};
}

int ErrorHandling::checkPort(char *port)
{
    int i = 0;

    while (port[i] != '\0') {
        if (port[i] < '0' || port[i] > '9')
            return 84;
        i++;
    }
    return 0;
}
