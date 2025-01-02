/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Arguments.cpp
*/

#include "Arguments.hpp"

#include <algorithm>
#include <string>

/**
 * @brief The help function when user specifies -h
 * 
 */
void Arguments::help()
{
    std::cout << "USAGE: ./r-type_server [OPTIONS]" << std::endl;
    std::cout << "OPTIONS:" << std::endl;
    std::cout << "\t-h, --help\t\t\tDisplay this help message" << std::endl;
    std::cout << "\t-p, --port=PORT\t\t\tSet the port of the server" << std::endl;
}

std::optional<std::variant<std::size_t, std::string>> Arguments::getArgument(const std::string &flag, char const *const *argv)
{
    for (std::size_t i = 0; argv[i]; i++)
    {
        if (flag == std::string(argv[i]) && argv[i + 1] != nullptr)
        {
            std::string argument = argv[i + 1];
            if (std::ranges::all_of(argument, isdigit))
            {
                return std::variant<std::size_t, std::string>(std::stoul(argument));
            }
            return std::variant<std::size_t, std::string>(argument);
        }
    }
    return std::nullopt;
}

/**
 * @brief Gets all the configured flags based on program parameters
 * 
 * @param argc 
 * @param argv 
 */
void Arguments::getArguments([[maybe_unused]]int argc, char const *const *argv)
{
    port_ = checkDefaultFlag(getArgument("-p", argv), port_);
    nbClients_ = checkDefaultFlag(getArgument("-c", argv), nbClients_);
}
