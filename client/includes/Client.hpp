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


#define WIDTH 800
#define HEIGHT 600
#define USER_CONFIG_FILEPATH ".config/client/user"

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
    std::size_t my_server_id_{};
};
