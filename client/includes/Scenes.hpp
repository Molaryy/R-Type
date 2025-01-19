/*
** EPITECH PROJECT, 2024
** r-type_client
** File description:
** Main.hpp
*/

#pragma once

#include "Client.hpp"

#define NB_MENU_BUTTONS 6

// Colors
#define COLOR_GREY {200, 200, 200, 255}
#define COLOR_DARK_BLUE {20, 82, 172, 255}
#define COLOR_WHITE {255, 255, 255, 255}

void lobbyCallback(Registry &r);
void lobbyPage(Registry &r, std::size_t lobby_id);
void settingsCallback(Registry &r);
void createMenuScene(Registry &r);
void exitButtonCallback(Registry &r);
void accessibilityCallback(Registry &r);
