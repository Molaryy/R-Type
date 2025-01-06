/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Arguments.hpp
*/

#pragma once

#include <cstddef>
#include <exception>
#include <iostream>
#include <optional>
#include <utility>
#include <variant>

#define PORT 8081
#define NB_CLIENTS 4

class Arguments {
public:
    void getArguments(int argc, char const *const *argv);

    [[nodiscard]] std::size_t getPort() const {
        return port_;
    }

    [[nodiscard]] std::size_t getNbClients() const {
        return nbClients_;
    }

    class ArgumentsException final : public std::exception {
    public:
        explicit ArgumentsException(std::string message) : message_(std::move(message)) {
        }

        [[nodiscard]] const char *what() const noexcept override {
            return message_.c_str();
        }

    private:
        std::string message_;
    };

private:
    template <typename T>
    T checkDefaultFlag(std::optional<std::variant<std::size_t, std::string>> argValue, T defaultValue) {
        if (argValue)
            if (auto *value = std::get_if<T>(&*argValue))
                return *value;
        return defaultValue;
    }

    static std::optional<std::variant<std::size_t, std::string>> getArgument(const std::string &flag, char const *const *argv);
    void parseArguments(int argc, char const *const *argv);
    static void help();

    std::size_t port_ = PORT;
    std::size_t nbClients_ = NB_CLIENTS;
};
