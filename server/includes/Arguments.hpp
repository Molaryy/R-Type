/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Arguments.hpp
*/

#pragma once

#include <iostream>
#include <vector>
#include <cstddef>
#include <any>
#include <exception>
#include <optional>
#include <variant>
#include <algorithm>

#define PORT 8081
#define NB_CLIENTS 4

class Arguments
{
    public:
        Arguments() = default;
        ~Arguments() = default;
        void getArguments(int argc, char const *const *argv);
        const std::size_t getPort() { return port_; }
        const std::size_t getNbClients() { return nbClients_; }
        class ArgumentsException : public std::exception
        {
            public:
                ArgumentsException(std::string const &message) : message_(message) {}
                ~ArgumentsException() = default;
                const char *what() const noexcept override { return message_.c_str(); }
            private:
                std::string message_;
        };
    private:
        /**
         * @brief Check if the argument is a flag (-c or -p)
         * 
         * @tparam T 
         * @param argValue 
         * @param defaultValue 
         * @return T 
         */
        template <typename T>
        T checkDefaultFlag(std::optional<std::variant<std::size_t, std::string>> argValue, T defaultValue)
        {
            if (argValue)
            {
                if (auto *value = std::get_if<T>(&*argValue))
                    return *value;
            }
            return defaultValue;
        }
        /**
         * @brief Get the Argument object
         * 
         * @param flag 
         * @param argv 
         * @return std::optional<std::variant<std::size_t, std::string>> 
         */
        std::optional<std::variant<std::size_t, std::string>> getArgument(const std::string &flag, char const *const *argv);
        void parseArguments(int argc, char const *const *argv);
        bool isStrNumber(const std::string &arg);
        void help();
        std::size_t port_ = PORT;
        std::size_t nbClients_ = NB_CLIENTS;
};
