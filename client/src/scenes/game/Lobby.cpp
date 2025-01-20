/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Lobby
*/

#include "Lobby.hpp"

#include "Client.hpp"
#include "Components.hpp"
#include "Components.hh"
#include "RTypeProtocol.hpp"
#include "Scenes.hpp"

bool Lobby::timeOut(const std::chrono::system_clock::time_point time_out_clock, Network::INetworkClient &network)
{
    Registry &r = Client::getInstance().getRegistry();
    const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    if (const std::chrono::duration<double> elapsed_seconds = now - time_out_clock; elapsed_seconds.count() > 3.0)
    {
        std::cerr << "Couldn't receive packet: time out" << std::endl;
        const entity_t popup = r.spawn_entity();
        r.add_component(popup, Components::RenderText("Connection to server timed out. Try again.", 20, true));

        r.add_component(popup, Position(200, 250));
        r.add_component(popup, Components::ColorText({255, 0, 0, 255}));
        r.add_component(popup, Components::ColorOverText({255, 0, 0, 255}, {255, 0, 0, 255}));
        r.add_component(popup, Components::ClickableText([](Registry &) {
            exit(0);
        }));
        return true;
    }
    std::vector<uint8_t> oldest_packet = network.getOldestPacket();
    if (oldest_packet.empty())
        return false;
    Network::Packet deserialized_packet(oldest_packet);
    Client::getInstance().getPacketHandler()(deserialized_packet);
    return false;
}

void Lobby::leaveLobby()
{
    bool received = false;
    Network::INetworkClient &network = Client::getInstance().getNetworkLib();
    Network::PacketHandler &packet_handler = Client::getInstance().getPacketHandler();

    packet_handler.setPacketCallback(Protocol::ACCEPT_LEAVE_LOBBY, [&received]([[maybe_unused]] const Network::Packet &packet) {
        received = true;
    });

    Network::Packet packetSended(Protocol::EmptyPacket(), Protocol::LEAVE_LOBBY);
    network.send(packetSended.serialize());

    const std::chrono::system_clock::time_point time_out_clock = std::chrono::system_clock::now();
    while (!received && !timeOut(time_out_clock, network));
}

std::vector<Protocol::LobbyDataPacket> Lobby::getLobbyList(Network::INetworkClient &network, Network::PacketHandler &packet_handler)
{
    bool get_lobby_list = false;
    std::size_t nb_lobby;
    std::vector<Protocol::LobbyDataPacket> lobby_data;

    packet_handler.setPacketCallback(Protocol::LOBBY_LIST, [&nb_lobby, &get_lobby_list, &network](const Network::Packet &packet)
    {
        get_lobby_list = true;

        auto [number_of_lobbys] = packet.getPayload<Protocol::LobbyListPacket>();
        nb_lobby = number_of_lobbys;

        for (std::size_t i = 0; i < nb_lobby; ++i)
        {
            Network::Packet packetSended(Protocol::AskLobbyDataPacket(i), Protocol::ASK_LOBBY_DATA);
            network.send(packetSended.serialize());
        }
    });

    packet_handler.setPacketCallback(Protocol::LOBBY_DATA, [&lobby_data](const Network::Packet &packet)
    {
        const auto data = packet.getPayload<Protocol::LobbyDataPacket>();
        lobby_data.push_back(data);
    });

    Network::Packet packet(Protocol::EmptyPacket(), Protocol::ASK_LOBBY_LIST);
    network.send(packet.serialize());

    const std::chrono::system_clock::time_point time_out_clock = std::chrono::system_clock::now();

    while (!get_lobby_list && !timeOut(time_out_clock, network))
    while (lobby_data.size() < nb_lobby && !timeOut(time_out_clock, network));
    return lobby_data;
}

Protocol::LobbyDataPacket Lobby::getLobbyData(const std::size_t lobby_id)
{
    Protocol::LobbyDataPacket lobby_data{};
    bool received_lobby_data = false;
    Network::INetworkClient &network = Client::getInstance().getNetworkLib();
    Network::PacketHandler &packet_handler = Client::getInstance().getPacketHandler();

    packet_handler.setPacketCallback(Protocol::LOBBY_DATA, [&lobby_data, &received_lobby_data](const Network::Packet &packet)
    {
        received_lobby_data = true;
        lobby_data = packet.getPayload<Protocol::LobbyDataPacket>();
    });

    Network::Packet packetSended(Protocol::AskLobbyDataPacket(lobby_id), Protocol::ASK_LOBBY_DATA);
    network.send(packetSended.serialize());

    const std::chrono::system_clock::time_point time_out_clock = std::chrono::system_clock::now();

    while (!received_lobby_data && !timeOut(time_out_clock, network));
    return lobby_data;
}

void Lobby::startLobby(Registry &r)
{
    Network::Packet packetSended(Protocol::EmptyPacket(), Protocol::ASK_START_GAME);
    Client::getInstance().getNetworkLib().send(packetSended.serialize());

    std::cout << "Starting lobby" << std::endl;
}

void Lobby::lobbyPage(Registry &r, const std::size_t lobby_id)
{
    r.clear_entities();
    const Protocol::LobbyDataPacket lobby = getLobbyData(lobby_id);
    Client::getInstance().lobby_id = lobby_id;

    constexpr Color white = COLOR_WHITE;
    constexpr Color grey = COLOR_GREY;
    constexpr Color darkBlue = COLOR_DARK_BLUE;

    entity_t e = r.spawn_entity();
    r.add_component(e, Components::RenderText(std::string("Lobby ") + std::to_string(lobby_id), 40, true));
    r.add_component(e, Position(50, 50));
    r.add_component(e, Components::ColorText(white));

    e = r.spawn_entity();
    r.add_component(e, Components::RenderText("Reload", 40, true));
    r.add_component(e, Position(600, 50));
    r.add_component(e, Components::ColorText(white));
    r.add_component(e, Components::ClickableText([this, lobby_id](Registry &reg) {
        this->lobbyPage(reg, lobby_id);
    }));
    r.add_component(e, Components::ColorOverText(darkBlue, grey));
    r.add_component(e, Components::MouseOverText(false));

    e = r.spawn_entity();
    r.add_component(e, Components::RenderText(std::string("Players ") + std::to_string(lobby.nb_players), 40, true));
    r.add_component(e, Position(300, 200));
    r.add_component(e, Components::ColorText(white));

    e = r.spawn_entity();
    r.add_component(e, Components::RenderText("Start Game", 60, true));
    r.add_component(e, Position(225, 300));
    r.add_component(e, Components::ColorText(white));
        r.add_component(e, Components::ClickableText([this, &r](Registry &) { this->startLobby(r); }));
r.add_component(e, Components::ColorOverText(darkBlue, grey));
    r.add_component(e, Components::MouseOverText(false));

    e = r.spawn_entity();
    r.add_component(e, Components::RenderText(std::string("Gamemode: ") + (lobby.game_mode ? "Endless" : "Campaign"), 30));
    r.add_component(e, Position(265, 400));

    r.add_component(e, Components::ColorText(white));
    r.add_component(e, Components::ClickableText([this, lobby_id](Registry &reg) {
        bool received_lobby_data = false;
        Network::INetworkClient &network = Client::getInstance().getNetworkLib();
        Network::PacketHandler &packet_handler = Client::getInstance().getPacketHandler();

        packet_handler.setPacketCallback(Protocol::LOBBY_DATA, [&received_lobby_data](Network::Packet &) {
            received_lobby_data = true;
        });

        Network::Packet packetSended(Protocol::EmptyPacket(), Protocol::CHANGE_GAME_MODE);
        network.send(packetSended.serialize());

        const std::chrono::system_clock::time_point time_out_clock = std::chrono::system_clock::now();

        while (!received_lobby_data && !timeOut(time_out_clock, network));
        lobbyPage(reg, lobby_id);
    }));
    r.add_component(e, Components::ColorOverText(darkBlue, grey));
    r.add_component(e, Components::MouseOverText(false));

    e = r.spawn_entity();
    r.add_component(e, Components::RenderText("Lobbies list", 20, true));
    r.add_component(e, Position(50, 550));
    r.add_component(e, Components::ColorText(white));
    r.add_component(e, Components::ClickableText([this](Registry &reg)
    {
        this->leaveLobby();
        lobbyCallback(reg);
    }));
    r.add_component(e, Components::ColorOverText(darkBlue, grey));
    r.add_component(e, Components::MouseOverText(false));
}

void Lobby::joinLobby(Registry &r, const std::function<void()> &send_packet)
{
    Network::INetworkClient &network = Client::getInstance().getNetworkLib();
    Network::PacketHandler &packet_handler = Client::getInstance().getPacketHandler();

    bool joined = false;
    std::size_t lobby_id = 0;

    packet_handler.setPacketCallback(Protocol::ACCEPT_LOBBY_JOIN, [&joined, &lobby_id](const Network::Packet &packet)
    {
        joined = true;
        auto [new_lobby_id, entity_id] = packet.getPayload<Protocol::AcceptLobbyJoinPacket>();
        lobby_id = new_lobby_id;
    });

    send_packet();

    const std::chrono::system_clock::time_point time_out_clock = std::chrono::system_clock::now();

    while (!joined && !timeOut(time_out_clock, network))
    lobbyPage(r, lobby_id);
}

void Lobby::lobbyCallback(Registry &r)
{
    Network::INetworkClient &network = Client::getInstance().getNetworkLib();
    Network::PacketHandler &packet_handler = Client::getInstance().getPacketHandler();
    std::vector<Protocol::LobbyDataPacket> lobby_list = getLobbyList(network, packet_handler);

    r.clear_entities();

    entity_t e = r.spawn_entity();

    constexpr Color white = COLOR_WHITE;
    constexpr Color grey = COLOR_GREY;
    constexpr Color darkBlue = COLOR_DARK_BLUE;

    r.add_component(e, Components::RenderText("Lobbies", 40, true));
    r.add_component(e, Position(50, 50));
    r.add_component(e, Components::ColorText(white));

    e = r.spawn_entity();
    r.add_component(e, Components::RenderText("Reload", 40, true));
    r.add_component(e, Position(600, 50));
    r.add_component(e, Components::ColorText(white));
    r.add_component(e, Components::ClickableText([this](Registry &reg) { this->lobbyCallback(reg); }));
    r.add_component(e, Components::ColorOverText(darkBlue, grey));
    r.add_component(e, Components::MouseOverText(false));
    r.add_component(e, Components::MouseOverTextSound(makeSound(NB_MENU_BUTTONS + 4 + RELOAD), NB_MENU_BUTTONS + 4 + RELOAD));

    e = r.spawn_entity();
    r.add_component(e, Components::RenderText("Menu", 20, true));
    r.add_component(e, Position(50, 550));
    r.add_component(e, Components::ColorText(white));
    r.add_component(e, Components::ClickableText(createMenuScene));
    r.add_component(e, Components::ColorOverText(darkBlue, grey));
    r.add_component(e, Components::MouseOverText(false));
    r.add_component(e, Components::MouseOverTextSound( makeSound(NB_MENU_BUTTONS + 4 + MENU), NB_MENU_BUTTONS + 4 + MENU));

    e = r.spawn_entity();
    r.add_component(e, Components::RenderText("Join Random Lobby", 20, true));
    r.add_component(e, Position(150, 550));
    r.add_component(e, Components::ColorText(white));
    r.add_component(e, Components::ClickableText([this, &network]([[maybe_unused]] Registry &registry)
    {
        joinLobby(registry, [this, &network]
        {
            Network::Packet packetSended(Protocol::EmptyPacket(), Protocol::JOIN_RANDOM_LOBBY);
            network.send(packetSended.serialize());
        });
    }));
    r.add_component(e, Components::ColorOverText(darkBlue, grey));
    r.add_component(e, Components::MouseOverText(false));
    r.add_component(e, Components::MouseOverTextSound( makeSound(NB_MENU_BUTTONS + 4 + JOIN_RANDOM_LOBBY), NB_MENU_BUTTONS + 4 + JOIN_RANDOM_LOBBY));

    e = r.spawn_entity();
    r.add_component(e, Components::RenderText("Join New Lobby", 20, true));
    r.add_component(e, Position(550, 550));
    r.add_component(e, Components::ColorText(white));
    r.add_component(e, Components::ClickableText([this, &network]([[maybe_unused]] Registry &registry)
    {
        joinLobby(registry, [&network]
        {
            Network::Packet packetSended(Protocol::EmptyPacket(), Protocol::JOIN_NEW_LOBBY);
            network.send(packetSended.serialize());
        });
    }));
    r.add_component(e, Components::ColorOverText(darkBlue, grey));
    r.add_component(e, Components::MouseOverText(false));
    r.add_component(e, Components::MouseOverTextSound( makeSound(NB_MENU_BUTTONS + 4 + JOIN_NEW_LOBBY), NB_MENU_BUTTONS + 4 + JOIN_NEW_LOBBY));

    const std::unordered_map<Protocol::LobbyState, std::string> state_string {
        {Protocol::OPEN, "Open"},
        {Protocol::FULL, "Full"},
        {Protocol::IN_GAME, "In game"},
        {Protocol::CLOSE, "Close"}
    };

    int i = 0;
    for (const auto &lobby : lobby_list)
    {
        entity_t button = r.spawn_entity();
        i++;
        r.add_component(button, Components::RenderText(std::string("Lobby ") + std::to_string(lobby.lobby_id)
                                                       + ": " + std::to_string(lobby.nb_players) + " player" + (lobby.nb_players > 1 ? "s " : " ") + ": Status "
                                                       + state_string.at(lobby.lobby_state), 20, true));
        r.add_component(button, Position(100, static_cast<float>(150 + i * 40)));
        r.add_component(button, Components::ColorText(grey));

        r.add_component(button, Components::ClickableText([this, lobby, &network]([[maybe_unused]] Registry &registry)
        {
            auto data = getLobbyData(lobby.lobby_id);
            if (data.lobby_state != Protocol::OPEN)
            {
                lobbyCallback(registry);
            }
            joinLobby(registry, [lobby, &network]
            {
                Network::Packet packetSended(Protocol::JoinLobbyPacket(lobby.lobby_id), Protocol::JOIN_LOBBY_BY_ID);
                network.send(packetSended.serialize());
            });
        }));
        r.add_component(button, Components::ColorOverText(darkBlue, grey));
        r.add_component(button, Components::MouseOverText(false));
        r.add_component(button, Components::MouseOverTextSound( makeSound(NB_MENU_BUTTONS + 4 + LOBBY), NB_MENU_BUTTONS + 4 + LOBBY));
    }
}
