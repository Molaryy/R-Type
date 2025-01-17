/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Score
*/

#pragma once

#include <string>
#include <unordered_map>

class Score
{
    public:
        Score(const std::string &filePath) { loadScores(filePath); }
        ~Score() = default;

        int getScore(const std::string &playerName) const;
        void addScore(const std::string &playerName, int score);
        void writeScores(const std::string &filePath);
    private:
        void loadScores(const std::string &filePath);
        std::unordered_map<std::string, int> scores_;
};