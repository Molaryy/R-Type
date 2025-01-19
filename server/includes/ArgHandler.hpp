/*
** EPITECH PROJECT, 2024
** R_Type
** File description:
** ArgHandler
*/

#ifndef ARGHANDLER_HPP_
#define ARGHANDLER_HPP_

#include <optional>
#include <string>
#include <vector>

#define PORT 4242
#define MAX_LOBBY 4
#define MAX_CLIENTS 10

class ArgHandler {
public:
    explicit ArgHandler(std::size_t max_args);

    static void display_help(const std::string_view &exec_name);
    static bool check_help(int ac, const std::string_view &exec_name, const std::string_view &first_arg);
    void verif_arg(int ac, const std::vector<std::string_view> &av);

    std::size_t port{PORT};
    std::size_t max_client{MAX_CLIENTS};
    std::size_t max_lobby{MAX_LOBBY};
    bool debug{false};
    bool debug_network{false};

private:
    static std::optional<std::string> getArgument_(const std::string &arg, const std::vector<std::string_view> &av);

    std::size_t max_args_ = 0;
};

#endif /* !ARGHANDLER_HPP_ */
