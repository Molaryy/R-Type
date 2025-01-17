/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Score
*/

#include "Score.hpp"

#include <fstream>
#include <algorithm>

int Score::getScore(const std::string &playerName) const
{
    if (!scores_.contains(playerName))
        return 0;
    return scores_.at(playerName);
}

void Score::loadScores(const std::string &filePath)
{
    std::ifstream file(filePath);
    std::string line;
    std::string playerName;
    int score;

    if (!file.is_open())
        return;

    while (std::getline(file, line))
    {
        if (line.find(":") == std::string::npos)
            continue;

        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
        line.erase(std::remove(line.begin(), line.end(), '\"'), line.end());

        playerName = line.substr(0, line.find(":"));
        score = std::stoi(line.substr(line.find(":") + 1));

        scores_[playerName] = score;
    }
    file.close();
}

void Score::addScore(const std::string &playerName, int score)
{
    scores_[playerName] = score;
}

void Score::writeScores(const std::string &filePath)
{
    std::ofstream file(filePath);

    if (!file.is_open())
        return;

    file << "{" << std::endl;
    for (auto it = scores_.begin(); it != scores_.end(); ++it)
    {
        file << "    \"" << it->first << "\": " << it->second;
        if (std::next(it) != scores_.end())
            file << ",";
        file << std::endl;
    }
    file << "}" << std::endl;
    file.close();
}
