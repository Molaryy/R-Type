/*
** EPITECH PROJECT, 2024
** r-type_client
** File description:
** Main.hpp
*/

#pragma once

#include "Client.hpp"

#define NB_MENU_BUTTONS 6
#define MENU_BUTTONS_TITLES {"Play", "Leaderboard", "Settings", "Credits", "Logout", "Exit"}
#define VOICE_BUTTONS_TITLES_FILEPATHS {"./assets/voice/Play.wav", "./assets/voice/Leaderboard.wav", "./assets/voice/Settings.wav", "./assets/voice/Credits.wav", "./assets/voice/Logout.wav", "./assets/voice/Exit.wav"}

#define PLAY_SOUND_ID 0
#define LEADERBOARD_SOUND_ID 1
#define SETTINGS_SOUND_ID 2
#define CREDITS_SOUND_ID 3
#define LOGOUT_SOUND_ID 4
#define EXIT_SOUND_ID 5


// Colors
#define COLOR_GREY {200, 200, 200, 255}
#define COLOR_DARK_BLUE {20, 82, 172, 255}
#define COLOR_WHITE {255, 255, 255, 255}

void lobbyCallback(Registry &r);
void settingsCallback(Registry &r);
void createMenuScene(Registry &r);
std::string getLocalUsername();
void createSignForm(Registry &r);
void exitButtonCallback(Registry &r);
