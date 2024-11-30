/*
** EPITECH PROJECT, 2024
** B-CPP-500-PAR-5-2-bsrtype-arthaud.poupard
** File description:
** Server.cpp
*/

#include "Server.hpp"

int server::Server::run() {
    _network.run();
    return 0;
}
