/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Arguments.cpp
*/

#include "Arguments.hpp"

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

/**
 * @brief Gets the argument based on a flag, it supports std::size_t and std::string
 * 
 * @param flag 
 * @param argv 
 * @return std::optional<std::variant<std::size_t, std::string>> 
 */
std::optional<std::variant<std::size_t, std::string>> Arguments::getArgument(const std::string &flag, char const *const *argv)
{
    for (std::size_t i = 0; argv[i]; i++)
    {
        if (std::strcmp(flag.c_str(), argv[i]) == 0 && argv[i + 1] != nullptr)
        {
            std::string argument = argv[i + 1];
            if (isStrNumber(argument)) {
                return std::variant<std::size_t, std::string>(std::stoul(argument));
            } else {
                return std::variant<std::size_t, std::string>(argument);
            }
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
void Arguments::getArguments(int argc, char const *const *argv)
{
    port_ = checkDefaultFlag(getArgument("-p", argv), port_);
    nbClients_ = checkDefaultFlag(getArgument("-c", argv), nbClients_);
}

/**
 * @brief Checks if the argument is set, if not it returns the default value
 * 
 * @param arg 
 * @return true 
 * @return false 
 */
bool Arguments::isStrNumber(const std::string &arg)
{
    return !arg.empty() && std::all_of(arg.begin(), arg.end(), ::isdigit);
}
