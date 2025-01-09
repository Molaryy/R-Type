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

class ArgHandler {
public:
    explicit ArgHandler(std::size_t max_args);

    static void display_help(const std::string_view &exec_name);
    static bool check_help(int ac, const std::string_view &exec_name, const std::string_view &first_arg);
    void verif_arg(int ac, const std::vector<std::string_view> &av);

    std::string ip{"127.0.0.1"};
    std::size_t port {4242};
private:
    static std::optional<std::string> getArgument_(const std::string &arg, const std::vector<std::string_view> &av);

    std::size_t max_args_ = 0;
};

#endif /* !ARGHANDLER_HPP_ */
