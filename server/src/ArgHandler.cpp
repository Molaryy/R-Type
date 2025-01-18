/*
** EPITECH PROJECT, 2024
** R_Type
** File description:
** Init
*/

#include "ArgHandler.hpp"
#include <ranges>

#include <iostream>
#include <optional>
#include <string>
#include <algorithm>


ArgHandler::ArgHandler(const std::size_t max_args) : max_args_(max_args) {
}

void ArgHandler::display_help(const std::string_view &exec_name) {
    std::cout << "USAGE: " << exec_name << " [OPTIONS]" << std::endl;
    std::cout << "OPTIONS:" << std::endl;
    std::cout << "\t-h|--help\t\t\tDisplay this help message" << std::endl;
    std::cout << "\t-p PORT\t\t\tSpecifies server port (default " << PORT << ")" << std::endl;
    std::cout << "\t-d\t\t\tDebug mode (log entities components every frame)" << std::endl;
    std::cout << "\t-l\t\t\tSpecifies max lobby authorized (don't put more than your thread number) (default " << MAX_LOBBY << " )" << std::endl;
    std::cout << "\t-c\t\t\tSpecifies max clients connected (don't put more than your thread number) (default " << MAX_CLIENTS << " )" << std::endl;
}

bool ArgHandler::check_help(const int ac, const std::string_view &exec_name, const std::string_view &first_arg) {
    if (ac == 2 && (first_arg == "-h" || first_arg == "--help")) {
        display_help(exec_name);
        return true;
    }
    return false;
}

void ArgHandler::verif_arg(const int ac, const std::vector<std::string_view> &av) {
    if (static_cast<std::size_t>(ac) > max_args_ + 1 || av.size() > max_args_ + 1)
        throw std::runtime_error(std::string(av[0]) + ": max args: " + std::to_string(max_args_));

    const std::optional<std::string> my_port = getArgument_("-p", av);
    if (my_port.has_value())
        port = std::stoi(my_port.value());

    const std::optional<std::string> my_max_client = getArgument_("-c", av);
    if (my_max_client.has_value())
        max_client = std::stoi(my_max_client.value());

    const std::optional<std::string> my_max_lobby = getArgument_("-l", av);
    if (my_max_lobby.has_value())
        max_lobby = std::stoi(my_max_lobby.value());

    if (std::ranges::find(av, "-d") != av.end())
        debug = true;
}

std::optional<std::string> ArgHandler::getArgument_(const std::string &arg, const std::vector<std::string_view> &av) {
    auto it = std::ranges::find(av, arg);
    if (it != av.end())
        ++it;
    return it == av.end() ? std::nullopt : std::optional(std::string(*it));
}
