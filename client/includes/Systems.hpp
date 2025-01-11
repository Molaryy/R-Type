/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** control system
*/

#pragma once

#include "Client.hpp"
#include "INetworkClient.hpp"
#include "PacketHandler.hpp"
#include "Registry.hh"
#include "Zipper.hh"
#include "Components.hpp"
#include "Components.hh"

namespace Systems {
    inline void networkReceiver([[maybe_unused]] Registry &r) {
        auto &core = Client::getInstance();
        auto &network = core.getNetworkLib();
        auto &packet_handler = core.getPacketHandler();

        for (std::vector<uint8_t> oldest_packet = network.getOldestPacket(); !oldest_packet.empty(); oldest_packet = network.getOldestPacket()) {
            Network::Packet deserialized_packet(oldest_packet);
            packet_handler(deserialized_packet);
        }
    }

    inline void drawAllTexts(Registry &r) {
        auto &texts = r.get_components<Components::RenderText>();
        auto &colors = r.get_components<Components::ColorText>();
        Graphic::IRenderer &renderer = Client::getInstance().getRenderer();

        for (auto &&[text, color] : Zipper(texts, colors)) {
            std::cout << "Drawing text: " << text.text << std::endl;
            renderer.drawText(text.text, text.x, text.y, text.fontSize, color.r, color.g, color.b, color.a);
        }
    }
}
