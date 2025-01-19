/*
** EPITECH PROJECT, 2024
** R_Type
** File description:
** Client
*/

#pragma once

#include <dylib.hpp>
#include <memory>

#include "INetworkClient.hpp"
#include "IRenderer.hpp"
#include "PacketHandler.hpp"
#include "Registry.hh"

#define SOUND_PLAYER_BULLET "assets/sounds/Fx/shot1.wav"
#define SOUND_ENEMY_BULLET "assets/sounds/Fx/shot2.wav"
#define SOUND_EXPLOSION "assets/sounds/Fx/explosion.wav"
#define SOUND_GAME_OVER "assets/sounds/Music/game-over.mp3"
#define SPACE_ASTEROIDS "assets/sounds/Music/space-asteroids.mp3"

class Client
{
public:
    Client(const Client &ref) = delete;
    void operator=(const Client &ref) = delete;

    ~Client();

    void run();

    static Client &createInstance(const std::string &ip, std::size_t port, bool debug);
    static Client &getInstance();

    Graphic::IRenderer &getRenderer() const;
    Network::INetworkClient &getNetworkLib() const;

    Network::PacketHandler &getPacketHandler();

    Registry &getRegistry();

    std::size_t my_server_id{};
    std::size_t lobby_id{};

    // methods for settings
    void toggleMusic();
    void playMusic();
    void stopMusic();
    void playSoundEffect(int soundID);
    void loadSounds();
    void toggleSoundEffects();

    void changeFPS();
    void changeResolution();

private:
    Client(const std::string &ip, std::size_t port, bool debug);

    bool connectToServer_(const std::string &ip, std::size_t port);
    void setupPacketHandler_();

    void setupSystems_();

    dylib graph_loader_;
    dylib network_loader_;
    std::unique_ptr<Graphic::IRenderer> renderer_;
    std::unique_ptr<Network::INetworkClient> network_lib_;

    Network::PacketHandler packet_handler_;
    Registry registry_;
    bool debug_;
    static std::unique_ptr<Client> instance_;

    // config for settings
    int musicID_ = -1;
    int playerBulletSoundID_ = -1;
    int enemyBulletSoundID_ = -1;
    int explosionSoundID_ = -1;
    int gameOverSoundID_ = -1;

    bool musicEnabled_ = true;
    bool soundEffectsEnabled_ = true;
    bool playGameOverMusic_ = false;

    int currentResolutionIndex_ = 0;
    int currentFPSIndex_ = 1;

    const std::vector<std::pair<int, int>> resolutions_ = {{800, 600}, {1280, 720}, {1920, 1080}};
    const std::vector<int> fpsOptions_ = {30, 60, 120};
};
