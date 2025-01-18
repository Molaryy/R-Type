/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Score
*/

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

class Score
{
    public:
    explicit Score(const std::string &filePath) { loadScores(filePath); }
        ~Score() = default;

        std::size_t getScore(const std::string &playerName) const;
        void addScore(const std::string &playerName, std::size_t score);
        void writeScores(const std::string &filePath);
        std::vector<std::pair<std::string, std::size_t>> getTopTen() const;
    private:
        void loadScores(const std::string &filePath);
        std::unordered_map<std::string, std::size_t> scores_;
};
