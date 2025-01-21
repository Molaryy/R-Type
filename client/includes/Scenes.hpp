/*
** EPITECH PROJECT, 2024
** r-type_client
** File description:
** Main.hpp
*/

#pragma once

#include "Client.hpp"

inline bool isAccessibilityOn = false;

enum MENU_BUTTONS : int {
    PLAY,
    LEADERBOARD,
    SETTINGS,
    CREDITS,
    LOGOUT,
    EXIT,
    ACCESSIBILITY,
    NB_MENU_BUTTONS
};

inline std::array<std::string, NB_MENU_BUTTONS> menuButtonsTitles = {
    "Play",
    "Leaderboard",
    "Settings",
    "Credits",
    "Logout",
    "Exit",
    "Accessibility"
};

inline std::array<std::string, NB_MENU_BUTTONS> voiceMenuButtonsTitleFilepaths = {
    "./assets/voice/Play.wav",
    "./assets/voice/Leaderboard.wav",
    "./assets/voice/Settings.wav",
    "./assets/voice/Credits.wav",
    "./assets/voice/Logout.wav",
    "./assets/voice/Exit.wav",
    "./assets/voice/Accessibility.wav"
};

enum LOBBY_BUTTONS : int {
    RELOAD,
    MENU,
    JOIN_RANDOM_LOBBY,
    JOIN_NEW_LOBBY,
    LOBBY,
    NB_LOBBY_BUTTONS
};

inline std::array<std::string, NB_LOBBY_BUTTONS> voiceLobbyButtonsTitleFilepaths = {
    "./assets/voice/Reload.wav",
    "./assets/voice/Menu.wav",
    "./assets/voice/JoinRandom.wav",
    "./assets/voice/JoinNew.wav",
    "./assets/voice/Lobby.wav",
};

// Colors
#define COLOR_GREY {200, 200, 200, 255}
#define COLOR_DARK_BLUE {20, 82, 172, 255}
#define COLOR_WHITE {255, 255, 255, 255}

void lobbyCallback(Registry &r);
void lobbyPage(Registry &r, std::size_t lobby_id);
void settingsCallback(Registry &r);
void createMenuScene(Registry &r);
std::string getLocalUsername();
void createSignForm(Registry &r);
void exitButtonCallback(Registry &r);
std::function<void(int)> makeSound(int soundID);
