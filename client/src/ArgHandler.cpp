/*
** EPITECH PROJECT, 2024
** R_Type
** File description:
** Init
*/

#include <iostream>
#include <string>
#include "ArgHandler.hpp"
#include <optional>

ArgHandler::ArgHandler(const std::size_t max_args) : max_args_(max_args) {
}

void ArgHandler::display_help(const std::string_view &exec_name) {
    std::cout << "USAGE: " << exec_name << "\n";
}

bool ArgHandler::check_help(const int ac, const std::string_view &exec_name, const std::string_view &first_arg) {
    if (ac == 2 && (first_arg == "-help" || first_arg == "-h" || first_arg == "--help")) {
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
        this->port = std::stoi(my_port.value());

    const std::optional<std::string> my_ip = getArgument_("-i", av);
    if (my_ip.has_value())
        this->ip = my_ip.value();
}

std::optional<std::string> ArgHandler::getArgument_(const std::string &arg, const std::vector<std::string_view> &av) {
    auto it = std::ranges::find(av, arg);
    if (it != av.end())
        it++;
    return it == av.end() ? std::nullopt : std::optional(std::string(*it));
}
