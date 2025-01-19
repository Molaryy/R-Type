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

    void toggleSoundEffects();
    void playSoundEffect(int soundID);

    void toggleColorBlindMode();
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
    bool musicEnabled_ = true;

    bool soundEffectsEnabled_ = true;
    int currentResolutionIndex_ = 0;
    int currentFPSIndex_ = 1;
    bool colorBlindModeEnabled_ = false;

    const std::vector<std::pair<int, int>> resolutions_ = {{800, 600}, {1280, 720}, {1920, 1080}};
    const std::vector<int> fpsOptions_ = {30, 60, 120};
};
