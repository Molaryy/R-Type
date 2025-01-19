/*
** EPITECH PROJECT, 2024
** R_Type
** File description:
** ArgHandler
*/

#pragma once

#include <optional>
#include <string>
#include <vector>

#define IP_ADDRESS "127.0.0.1"
#define PORT 4242

class ArgHandler {
public:
    explicit ArgHandler(const std::size_t max_args) : max_args_(max_args) {}

    static void display_help(const std::string_view &exec_name);
    static bool check_help(int ac, const std::string_view &exec_name, const std::string_view &first_arg);
    void verif_arg(int ac, const std::vector<std::string_view> &av);

    std::string ip{IP_ADDRESS};
    std::size_t port{PORT};
    bool debug{false};

private:
    static std::optional<std::string> getArgument_(const std::string &arg, const std::vector<std::string_view> &av);

    std::size_t max_args_ = 0;
};
