/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Lobby
*/

#include "Lobby.hpp"

#include "Client.hpp"
#include "Components.hpp"
#include "RTypeProtocol.hpp"
#include "Scenes.hpp"

std::vector<Protocol::LobbyDataPacket> getLobbyList(Network::INetworkClient &network, Network::PacketHandler &packet_handler) {
    bool get_lobby_list = false;
    std::size_t nb_lobby;
    std::vector<Protocol::LobbyDataPacket> lobby_data;

    packet_handler.setPacketCallback(Protocol::LOBBY_LIST, [&nb_lobby, &get_lobby_list, &network](const Network::Packet &packet) {
        get_lobby_list = true;

        auto [number_of_lobbys] = packet.getPayload<Protocol::LobbyListPacket>();
        nb_lobby = number_of_lobbys;

        for (std::size_t i = 0; i < nb_lobby; ++i) {
            Network::Packet packetSended(Protocol::AskLobbyDataPacket(i), Protocol::ASK_LOBBY_DATA);
            network.send(packetSended.serialize());
        }
    });

    packet_handler.setPacketCallback(Protocol::LOBBY_DATA, [&lobby_data](const Network::Packet &packet) {
        const auto data = packet.getPayload<Protocol::LobbyDataPacket>();
        lobby_data.push_back(data);
    });

    Network::Packet packet(Protocol::EmptyPacket(), Protocol::ASK_LOBBY_LIST);
    network.send(packet.serialize());

    const std::chrono::system_clock::time_point time_out_clock = std::chrono::system_clock::now();

    while (!get_lobby_list) {
        const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        if (const std::chrono::duration<double> elapsed_seconds = now - time_out_clock; elapsed_seconds.count() > 3.0)
            throw std::runtime_error("Can't get lobby list: time out");
        std::vector<uint8_t> oldest_packet = network.getOldestPacket();
        if (oldest_packet.empty())
            continue;
        Network::Packet deserialized_packet(oldest_packet);
        packet_handler(deserialized_packet);
    }
    while (lobby_data.size() < nb_lobby) {
        const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        if (const std::chrono::duration<double> elapsed_seconds = now - time_out_clock; elapsed_seconds.count() > 3.0)
            throw std::runtime_error("Can't get lobby data: time out");
        std::vector<uint8_t> oldest_packet = network.getOldestPacket();
        if (oldest_packet.empty())
            continue;
        Network::Packet deserialized_packet(oldest_packet);
        packet_handler(deserialized_packet);
    }
    return lobby_data;
}

Protocol::LobbyDataPacket getLobbyData(const std::size_t lobby_id) {
    Protocol::LobbyDataPacket lobby_data{};
    bool received_lobby_data = false;
    Network::INetworkClient &network = Client::getInstance().getNetworkLib();
    Network::PacketHandler &packet_handler = Client::getInstance().getPacketHandler();

    packet_handler.setPacketCallback(Protocol::LOBBY_DATA, [&lobby_data, &received_lobby_data](const Network::Packet &packet) {
        received_lobby_data = true;
        lobby_data = packet.getPayload<Protocol::LobbyDataPacket>();
    });

    Network::Packet packetSended(Protocol::AskLobbyDataPacket(lobby_id), Protocol::ASK_LOBBY_DATA);
    network.send(packetSended.serialize());

    const std::chrono::system_clock::time_point time_out_clock = std::chrono::system_clock::now();

    while (!received_lobby_data) {
        const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        if (const std::chrono::duration<double> elapsed_seconds = now - time_out_clock; elapsed_seconds.count() > 3.0)
            throw std::runtime_error("Can't get lobby list: time out");
        std::vector<uint8_t> oldest_packet = network.getOldestPacket();
        if (oldest_packet.empty())
            continue;
        Network::Packet deserialized_packet(oldest_packet);
        packet_handler(deserialized_packet);
    }
    return lobby_data;
}

void startLobby(Registry &r) {
    r.clear_entities();

    Network::Packet packetSended(Protocol::EmptyPacket(), Protocol::ASK_START_GAME);
    Client::getInstance().getNetworkLib().send(packetSended.serialize());

    std::cout << "Starting lobby" << std::endl;
}

void lobbyPage(Registry &r, const std::size_t lobby_id) {
    r.clear_entities();
    const Protocol::LobbyDataPacket lobby = getLobbyData(lobby_id);

    constexpr Color white = COLOR_WHITE;
    constexpr Color grey = COLOR_GREY;
    constexpr Color darkBlue = COLOR_DARK_BLUE;

    entity_t e = r.spawn_entity();
    r.add_component(e, Components::RenderText(std::string("Lobby ") + std::to_string(lobby_id), 50, 50, 40));
    r.add_component(e, Components::ColorText(white));

    e = r.spawn_entity();
    r.add_component(e, Components::RenderText("Reload", 600, 50, 40));
    r.add_component(e, Components::ColorText(white));
    r.add_component(e, Components::ClickableText([lobby_id](Registry &reg) {
        lobbyPage(reg, lobby_id);
    }));
    r.add_component(e, Components::ColorOverText(darkBlue, grey, false));

    e = r.spawn_entity();
    r.add_component(e, Components::RenderText(std::string("Players ") + std::to_string(lobby.nb_players), 300, 200, 40));
    r.add_component(e, Components::ColorText(white));

    e = r.spawn_entity();
    r.add_component(e, Components::RenderText("Start Game", 230, 350, 60));
    r.add_component(e, Components::ColorText(white));
    r.add_component(e, Components::ClickableText(startLobby));
    r.add_component(e, Components::ColorOverText(darkBlue, grey, false));

    e = r.spawn_entity();
    r.add_component(e, Components::RenderText("Lobby list", 50, 550, 20));
    r.add_component(e, Components::ColorText(white));
    r.add_component(e, Components::ClickableText(lobbyCallback));
    r.add_component(e, Components::ColorOverText(darkBlue, grey, false));
}

void joinLobby(Registry &r, const std::function<void()> &send_packet) {
    Network::INetworkClient &network = Client::getInstance().getNetworkLib();
    Network::PacketHandler &packet_handler = Client::getInstance().getPacketHandler();

    bool joined = false;
    std::size_t lobby_id = 0;


    packet_handler.setPacketCallback(Protocol::ACCEPT_LOBBY_JOIN, [&joined, &lobby_id](const Network::Packet &packet) {
        joined = true;
        auto [new_lobby_id, entity_id] = packet.getPayload<Protocol::AcceptLobbyJoinPacket>();
        lobby_id = new_lobby_id;
    });

    send_packet();

    const std::chrono::system_clock::time_point time_out_clock = std::chrono::system_clock::now();

    while (!joined) {
        const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        if (const std::chrono::duration<double> elapsed_seconds = now - time_out_clock; elapsed_seconds.count() > 3.0)
            throw std::runtime_error("Can't join or start: time out");
        std::vector<uint8_t> oldest_packet = network.getOldestPacket();
        if (oldest_packet.empty())
            continue;
        Network::Packet deserialized_packet(oldest_packet);
        packet_handler(deserialized_packet);
    }
    lobbyPage(r, lobby_id);
}

void lobbyCallback(Registry &r) {
    Network::INetworkClient &network = Client::getInstance().getNetworkLib();
    Network::PacketHandler &packet_handler = Client::getInstance().getPacketHandler();
    std::vector<Protocol::LobbyDataPacket> lobby_list = getLobbyList(network, packet_handler);

    r.clear_entities();

    entity_t e = r.spawn_entity();

    constexpr Color white = COLOR_WHITE;
    constexpr Color grey = COLOR_GREY;
    constexpr Color darkBlue = COLOR_DARK_BLUE;

    r.add_component(e, Components::RenderText("Lobbies", 50, 50, 40));
    r.add_component(e, Components::ColorText(white));

    e = r.spawn_entity();
    r.add_component(e, Components::RenderText("Reload", 600, 50, 40));
    r.add_component(e, Components::ColorText(white));
    r.add_component(e, Components::ClickableText(lobbyCallback));
    r.add_component(e, Components::ColorOverText(darkBlue, grey, false));

    e = r.spawn_entity();
    r.add_component(e, Components::RenderText("Menu", 50, 550, 20));
    r.add_component(e, Components::ColorText(white));
    r.add_component(e, Components::ClickableText(createMenuScene));
    r.add_component(e, Components::ColorOverText(darkBlue, grey, false));

    e = r.spawn_entity();
    r.add_component(e, Components::RenderText("Join Random Lobby", 150, 550, 20));
    r.add_component(e, Components::ColorText(white));
    r.add_component(e, Components::ClickableText([&network]([[maybe_unused]] Registry &registry) {
        joinLobby(registry, [&network] {
            Network::Packet packetSended(Protocol::EmptyPacket(), Protocol::JOIN_RANDOM_LOBBY);
            network.send(packetSended.serialize());
        });
    }));
    r.add_component(e, Components::ColorOverText(darkBlue, grey, false));

    e = r.spawn_entity();
    r.add_component(e, Components::RenderText("Join New Lobby", 550, 550, 20));
    r.add_component(e, Components::ColorText(white));
    r.add_component(e, Components::ClickableText([&network]([[maybe_unused]] Registry &registry) {
        joinLobby(registry, [&network] {
            Network::Packet packetSended(Protocol::EmptyPacket(), Protocol::JOIN_NEW_LOBBY);
            network.send(packetSended.serialize());
        });
    }));
    r.add_component(e, Components::ColorOverText(darkBlue, grey, false));

    const std::unordered_map<Protocol::LobbyState, std::string> state_string{
        {Protocol::OPEN, "Open"},
        {Protocol::FULL, "Full"},
        {Protocol::IN_GAME, "In game"},
        {Protocol::CLOSE, "Close"}
    };

    int i = 0;
    for (const auto &lobby : lobby_list) {
        entity_t button = r.spawn_entity();
        i++;
        r.add_component(button, Components::RenderText(std::string("Lobby ") + std::to_string(lobby.lobby_id)
                                                       + ": " + std::to_string(lobby.nb_players) + " player" + (lobby.nb_players > 1 ? "s " : " ") + ": Status "
                                                       + state_string.at(lobby.lobby_state), 100, 150 + i * 40, 20));
        r.add_component(button, Components::ColorText(grey));
        r.add_component(button, Components::ClickableText([lobby, &network]([[maybe_unused]] Registry &registry) {
            joinLobby(registry, [lobby, &network] {
                Network::Packet packetSended(Protocol::JoinLobbyPacket(lobby.lobby_id), Protocol::JOIN_LOBBY_BY_ID);
                network.send(packetSended.serialize());
            });
        }));
        r.add_component(button, Components::ColorOverText(darkBlue, grey, false));
    }
}
