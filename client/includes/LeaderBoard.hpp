/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** LeaderBoard.hpp
*/

#pragma once

#include "RTypeProtocol.hpp"

class LeaderBoard {
public:
    static Protocol::ScoreboardPacket getScoreboard();
    static Protocol::ScoreboardIdPacket getIdScoreboard(const std::string& name);
private:
};
