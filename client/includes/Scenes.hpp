/*
** EPITECH PROJECT, 2024
** r-type_client
** File description:
** Main.hpp
*/

#pragma once

#include "Client.hpp"

#define NB_MENU_BUTTONS 5

// Colors
#define COLOR_GREY {200, 200, 200, 255}
#define COLOR_DARK_BLUE {20, 82, 172, 255}
#define COLOR_WHITE {255, 255, 255, 255}


void newGameCallback(Registry &r);
void settingsCallback(Registry &r);
void createMenuScene(Registry &r);
